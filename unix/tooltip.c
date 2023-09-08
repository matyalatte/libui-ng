#include "unix/uipriv_unix.h"

uintptr_t uiTooltipSetSpinbox(uiSpinbox* s, const char* text) {
    return uiTooltipSetControl(uiControl(s), text);
}

uintptr_t uiTooltipSetControl(uiControl* c, const char* text) {
	gtk_widget_set_tooltip_text(GTK_WIDGET(uiControlHandle(c)), text);
	return NULL;
}

void uiTooltipDestroy(uintptr_t tooltip) {}
