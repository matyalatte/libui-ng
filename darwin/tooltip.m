#include "darwin/uipriv_darwin.h"

uintptr_t uiTooltipSetSpinbox(uiSpinbox* s, const char* text) {
    return uiTooltipSetControl(uiControl(s), text);
}

uintptr_t uiTooltipSetControl(uiControl* c, const char* text) {
    NSView* view = (NSView*)uiControlHandle(c);
    view.toolTip = uiprivToNSString(text);
    return NULL;
}

void uiTooltipDestroy(uintptr_t tooltip) {}
