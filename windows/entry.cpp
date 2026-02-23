// 8 april 2015
#include "uipriv_windows.hpp"

struct uiEntry {
	uiWindowsControl c;
	HWND hwnd;
	void (*onChanged)(uiEntry *, void *);
	void *onChangedData;
	BOOL inhibitChanged;
	void (*onFilesDropped)(uiEntry *, int, char**, void *);
	void *onFilesDroppedData;
	int acceptDrops;
	int placeholderLen;
};

static BOOL onWM_COMMAND(uiControl *c, HWND hwnd, WORD code, LRESULT *lResult)
{
	uiEntry *e = uiEntry(c);

	if (code != EN_CHANGE)
		return FALSE;
	if (e->inhibitChanged)
		return FALSE;
	(*(e->onChanged))(e, e->onChangedData);
	*lResult = 0;
	return TRUE;
}

static BOOL onWM_DROPFILES(uiControl *c, HWND hwnd, HDROP hdrop, LRESULT *lResult)
{
	uiEntry *e = uiEntry(c);

	int count = DragQueryFile(hdrop, -1, NULL, 0);

	char** names = (char**)uiprivAlloc((count + 1) * sizeof(char*), "char*[] names");
	char** start = names;
	wchar_t* file;
	for (int i = 0; i < count; i++) {
		size_t size = DragQueryFile(hdrop, i, NULL, 0);
		file = (wchar_t*)uiprivAlloc((size + 1) * sizeof(wchar_t), "wchar_t[] file");
		DragQueryFile(hdrop, i, file, size + 1);
		*names = toUTF8(file);
		uiprivFree(file);
		names++;
	}
	DragFinish(hdrop);

	(*(e->onFilesDropped))(e, count, start, e->onFilesDroppedData);

	names = start;
	for (int i = 0; i < count; i++) {
		uiprivFree(*names);
		names++;
	}
	uiprivFree(start);
	*lResult = 0;
	return TRUE;
}

static void uiEntryDestroy(uiControl *c)
{
	uiEntry *e = uiEntry(c);

	uiWindowsUnregisterWM_COMMANDHandler(e->hwnd);
	uiWindowsUnregisterWM_DROPFILESHandler(e->hwnd);
	uiprivDestroyTooltip(c);
	uiWindowsEnsureDestroyWindow(e->hwnd);
	uiFreeControl(uiControl(e));
}

uiWindowsControlAllDefaultsExceptDestroy(uiEntry)

// from http://msdn.microsoft.com/en-us/library/windows/desktop/dn742486.aspx#sizingandspacing
#define entryWidth 107 /* this is actually the shorter progress bar width, but Microsoft only indicates as wide as necessary */
#define entryHeight 14

static void uiEntryMinimumSize(uiWindowsControl *c, int *width, int *height)
{
	uiEntry *e = uiEntry(c);
	uiWindowsSizing sizing;
	int x, y;

	x = entryWidth;
	y = entryHeight;
	uiWindowsGetSizing(e->hwnd, &sizing);
	uiWindowsSizingDlgUnitsToPixels(&sizing, &x, &y);
	*width = x;
	*height = y;
}

static void defaultOnChanged(uiEntry *e, void *data)
{
	// do nothing
}

static void defaultOnFilesDropped(uiEntry *e, int count, char** names, void *data)
{
	// do nothing
}

char *uiEntryText(uiEntry *e)
{
	return uiWindowsWindowText(e->hwnd);
}

void uiEntrySetText(uiEntry *e, const char *text)
{
	int l;
	// doing this raises an EN_CHANGED
	e->inhibitChanged = TRUE;
	uiWindowsSetWindowText(e->hwnd, text);
	l = (int)strlen(text);
	if (GetFocus() == e->hwnd)
		Edit_SetSel(e->hwnd, l, l);
	e->inhibitChanged = FALSE;
	// don't queue the control for resize; entry sizes are independent of their contents
}

void uiEntryOnChanged(uiEntry *e, void (*f)(uiEntry *, void *), void *data)
{
	e->onChanged = f;
	e->onChangedData = data;
}

void uiEntryOnFilesDropped(uiEntry *e, void (*f)(uiEntry *, int, char**, void *), void *data)
{
	e->onFilesDropped = f;
	e->onFilesDroppedData = data;
}

int uiEntryReadOnly(uiEntry *e)
{
	return (getStyle(e->hwnd) & ES_READONLY) != 0;
}

void uiEntrySetReadOnly(uiEntry *e, int readonly)
{
	if (Edit_SetReadOnly(e->hwnd, readonly) == 0)
		logLastError(L"error setting uiEntry read-only state");
}

int uiEntryAcceptDrops(uiEntry *e)
{
	return e->acceptDrops;
}

void uiEntrySetAcceptDrops(uiEntry * e, int accept)
{
	e->acceptDrops = accept != 0;
	DragAcceptFiles(e->hwnd, accept);
}

char *uiEntryPlaceholder(uiEntry *e)
{
	int size = e->placeholderLen + 1;
	WCHAR *wtext = (wchar_t*)uiprivAlloc(size * sizeof(wchar_t), "wchar_t[] wtext");

	if (!SendMessageW(e->hwnd, EM_GETCUEBANNER, (WPARAM)wtext, size)) {
		logLastError(L"error getting placeholder text");
	}
	char *text;
	text = toUTF8(wtext);
	uiprivFree(wtext);
	return text;
}

void uiEntrySetPlaceholder(uiEntry *e, const char *text)
{
	WCHAR *wtext;
	wtext = toUTF16(text);
	e->placeholderLen = wcslen(wtext);
	if (!SendMessageW(e->hwnd, EM_SETCUEBANNER, FALSE, (LPARAM)wtext))
		logLastError(L"error setting placeholder text");
	uiprivFree(wtext);
}

static LRESULT CALLBACK entrySubProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	LRESULT lResult;

	switch (uMsg) {
	case WM_DROPFILES:
		return runWM_DROPFILES(wParam, (LPARAM)hwnd, &lResult);
		break;
	default:
		break;
	}
	return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}

static uiEntry *finishNewEntry(DWORD style)
{
	uiEntry *e;

	uiWindowsNewControl(uiEntry, e);

	e->hwnd = uiWindowsEnsureCreateControlHWND(WS_EX_CLIENTEDGE,
		L"edit", L"",
		style | ES_AUTOHSCROLL | ES_LEFT | ES_NOHIDESEL | WS_TABSTOP,
		hInstance, NULL,
		TRUE);

	if (SetWindowSubclass(e->hwnd, entrySubProc, 0, (DWORD_PTR) e) == FALSE)
		logLastError(L"error subclassing entry to handle parent messages");

	uiWindowsRegisterWM_COMMANDHandler(e->hwnd, onWM_COMMAND, uiControl(e));
	uiWindowsRegisterWM_DROPFILESHandler(e->hwnd, onWM_DROPFILES, uiControl(e));
	uiEntryOnChanged(e, defaultOnChanged, NULL);
	uiEntryOnFilesDropped(e, defaultOnFilesDropped, NULL);

	return e;
}

uiEntry *uiNewEntry(void)
{
	return finishNewEntry(0);
}

uiEntry *uiNewPasswordEntry(void)
{
	return finishNewEntry(ES_PASSWORD);
}

uiEntry *uiNewSearchEntry(void)
{
	uiEntry *e;
	HRESULT hr;

	e = finishNewEntry(0);

	hr = SetWindowTheme(e->hwnd, L"SearchBoxEditComposited", NULL);
	if (hr != S_OK || !IsAppThemed()) {
		//TODO log: Failed to apply search box theme.
	}

	return e;
}
