// 16 august 2015
#include "uipriv_haiku.hpp"

static void defaultCommitShow(uiControl *c)
{
	BView *view;

	view = (BView *) uiControlHandle(c);
	view->Show();
}

static void defaultCommitHide(uiControl *c)
{
	BView *view;

	view = (BView *) uiControlHandle(c);
	view->Hide();
}

void osCommitEnable(uiControl *c)
{
	// TODO this might need to be per-widget
}

void osCommitDisable(uiControl *c)
{
	// TODO this might need to be per-widget
}

void uiHaikuFinishControl(uiControl *c)
{
	c->Show = defaultCommitShow;
	c->Hide = defaultCommitHide;
	c->Enable = osCommitEnable;
	c->Disable = osCommitDisable;
}

// TODO make a signature for uiHaikuControl
#define uiUnixControlSignature 0x556E6978

uiHaikuControl *uiHaikuAllocControl(size_t n, uint32_t typesig, const char *typenamestr)
{
	return uiHaikuControl(uiAllocControl(n, uiUnixControlSignature, typesig, typenamestr));
}
