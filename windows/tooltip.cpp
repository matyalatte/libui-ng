#include "windows/uipriv_windows.hpp"

static uintptr_t CreateTooltip(HWND hparent, const wchar_t* text) {
    HWND hwndTT = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
        WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        hparent, NULL, hInstance, NULL);

    if (!hwndTT) {
        printf("ERROR: Failed to create tooltip window.");
        return NULL;
    }

    SetWindowPos(hwndTT, HWND_TOPMOST, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

    TTTOOLINFO ti = { 0 };
    ti.cbSize = sizeof(ti);
    ti.uFlags = TTF_SUBCLASS;
    ti.hwnd = hparent;
    ti.hinst = hInstance;
    ti.lpszText = (LPWSTR)text;
    GetClientRect(hparent, &ti.rect);
    ti.rect.right = 400;

    if (!SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM)&ti)) {
        logLastError(L"Failed to set rect to tooltip window.");
        uiWindowsEnsureDestroyWindow(hwndTT);
        return NULL;
    }
    return (uintptr_t)hwndTT;
}

uintptr_t uiTooltipSetSpinbox(uiSpinbox* s, const char* text) {
    HWND child = FindWindowExW((HWND)uiControlHandle(uiControl(s)), NULL, L"edit", NULL);
    if (!child) {
        logLastError(L"Failed to get text entry from spinbox.");
        return NULL;
    }
    wchar_t* wtext = toUTF16(text);
    uintptr_t tooltip = CreateTooltip(child, wtext);
    uiprivFree(wtext);
    return tooltip;
}

uintptr_t uiTooltipSetControl(uiControl* c, const char* text) {
    wchar_t* wtext = toUTF16(text);
    uintptr_t tooltip = CreateTooltip((HWND)uiControlHandle(c), wtext);
    uiprivFree(wtext);
    return tooltip;
}

void uiTooltipDestroy(uintptr_t tooltip) {
    uiWindowsEnsureDestroyWindow((HWND)tooltip);
}
