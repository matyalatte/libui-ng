// 11 april 2015
#include "uipriv_windows.hpp"

struct uiLabel {
	uiWindowsControl c;
	HWND hwnd;
	COLORREF color;
	WCHAR* wtext;
};

static void uiLabelDestroy(uiControl *c)
{
	uiLabel *l = uiLabel(c);

	if (l->wtext)
		uiprivFree(l->wtext);
	uiWindowsUnregisterWM_CTLCOLORSTATICHandler(l->hwnd);
	uiWindowsEnsureDestroyWindow(l->hwnd);
	uiFreeControl(c);
}

uiWindowsControlAllDefaultsExceptDestroy(uiLabel)

static void uiLabelMinimumSize(uiWindowsControl *c, int *width, int *height)
{
	uiLabel *l = uiLabel(c);
	uiWindowsSizing sizing;
	int y;

	*width = uiWindowsWindowTextWidth(l->hwnd);
	y = uiWindowsWindowTextHeight(l->hwnd);
	uiWindowsGetSizing(l->hwnd, &sizing);
	uiWindowsSizingDlgUnitsToPixels(&sizing, NULL, &y);
	*height = y;
}

char *uiLabelText(uiLabel *l)
{
	return uiWindowsWindowText(l->hwnd);
}

void uiLabelSetText(uiLabel *l, const char *text)
{
	if (l->wtext)
		uiprivFree(l->wtext);
	l->wtext = toUTF16(text);
	uiWindowsSetWindowText(l->hwnd, text);
	// changing the text might necessitate a change in the label's size
	uiWindowsControlMinimumSizeChanged(uiWindowsControl(l));
}

void uiLabelSetTextColor(uiLabel *l, double r, double g, double b)
{
	BYTE red = (BYTE)(r * 0xFF);
	BYTE green = (BYTE)(g * 0xFF);
	BYTE blue = (BYTE)(b * 0xFF);
	l->color = RGB(red, green, blue);
}

static BOOL onWM_CTLCOLORSTATIC(uiControl *c, HWND hwnd, HDC hdc, LRESULT *lResult)
{
	if (!uiWindowsUseLegacyRenderer())
		return TRUE;
	uiLabel *l = uiLabel(c);
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, l->color);
	return TRUE;
}

static LRESULT CALLBACK labelSubProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	uiLabel *l = uiLabel(dwRefData);
	uiControl *c = uiControl(l);
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;

	switch (uMsg) {
	case WM_PAINT:
		if (uiWindowsUseLegacyRenderer())
			break;
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		uiprivDrawTextToControl(c, hdc, &rect, l->wtext, l->color, FALSE);
		return TRUE;
		break;
	default:
		break;
	}
	return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}

uiLabel *uiNewLabel(const char *text)
{
	uiLabel *l;
	WCHAR *wtext;

	uiWindowsNewControl(uiLabel, l);

	l->wtext = toUTF16(text);
	l->hwnd = uiWindowsEnsureCreateControlHWND(0,
		L"static", l->wtext,
		// SS_LEFTNOWORDWRAP clips text past the end; SS_NOPREFIX avoids accelerator translation
		// controls are vertically aligned to the top by default (thanks Xeek in irc.freenode.net/#winapi)
		SS_LEFTNOWORDWRAP | SS_NOPREFIX,
		hInstance, NULL,
		TRUE);
	l->color = 0;

	uiWindowsRegisterWM_CTLCOLORSTATICHandler(l->hwnd, onWM_CTLCOLORSTATIC, uiControl(l));

	if (SetWindowSubclass(l->hwnd, labelSubProc, 0, (DWORD_PTR) l) == FALSE)
		logLastError(L"error subclassing label to handle parent messages");

	return l;
}
