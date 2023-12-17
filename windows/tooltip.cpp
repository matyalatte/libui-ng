#include "uipriv_windows.hpp"

// This doesn't work for containers
static HWND createTooltipForControl(HWND hparent, const wchar_t* text) {
	HWND hwndTT = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
		WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		hparent, NULL, hInstance, NULL);

	if (!hwndTT) {
		logLastError(L"Failed to create tooltip window.");
		return NULL;
	}

	SetWindowPos(hwndTT, HWND_TOPMOST, 0, 0, 0, 0,
				 SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

	TTTOOLINFO ti = { 0 };
	ti.cbSize = sizeof(ti);
	ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
	ti.uId = (UINT_PTR) hparent;
	ti.hinst = hInstance;
	ti.lpszText = (LPWSTR) text;

	if (!SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM) &ti)) {
		logLastError(L"Failed to set rect to tooltip window.");
		uiWindowsEnsureDestroyWindow(hwndTT);
		return NULL;
	}
	return hwndTT;
}

void uiControlSetTooltip(uiControl *c, const char *tooltip) {
	if (tooltip == NULL) {
		uiprivDestroyTooltip(c);
		return;
	}

	wchar_t *wtext = toUTF16(tooltip);
	HWND hparent = (HWND) uiControlHandle(c);
	HWND child = NULL;
	void *ptr = NULL;
	std::vector<HWND> *tooltip_vec;
	HWND hwndTT;

	switch (c->TypeSignature) {
	case uiSpinboxSignature:
		child = FindWindowExW(hparent, NULL, L"edit", NULL);
		if (!child) {
			logLastError(L"Failed to get text entry from spinbox.");
			break;
		}
		ptr = createTooltipForControl(child, wtext);
		break;
	case uiRadioButtonsSignature:
		// This works only for existing buttons.
		// You should run uiRadioButtonsAppend before calling uiControlSetTooltip.
		tooltip_vec = new std::vector<HWND>(0);
		child = FindWindowExW(hparent, NULL, L"button", NULL);
		while (child != NULL) {
			hwndTT = createTooltipForControl(child, wtext);
			if (hwndTT != NULL)
				tooltip_vec->push_back(hwndTT);
			child = FindWindowExW(hparent, child, L"button", NULL);
		}
		ptr = tooltip_vec;
		break;
	default:
		ptr = createTooltipForControl(hparent, wtext);
	}

	uiprivFree(wtext);
	uiWindowsControl(c)->tooltip = ptr;
}

void uiprivDestroyTooltip(uiControl* c) {
	void *tooltip = uiWindowsControl(c)->tooltip;
	std::vector<HWND> *tooltip_vec;

	if (tooltip == NULL) return;

	switch (c->TypeSignature) {
	case uiRadioButtonsSignature:
		tooltip_vec = (std::vector<HWND>*) tooltip;
		for (HWND hwndTT : *tooltip_vec) {
			uiWindowsEnsureDestroyWindow(hwndTT);
		}
		delete tooltip_vec;
		break;
	default:
		uiWindowsEnsureDestroyWindow((HWND) tooltip);
	}
	uiWindowsControl(c)->tooltip = NULL;
}
