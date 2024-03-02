// 18 november 2015
#include "uipriv_haiku.hpp"

struct uiProgressBar {
	uiHaikuControl c;
	BStatusBar *pbar;
};

uiHaikuDefineControl(
	uiProgressBar,							// type name
	pbar									// handle
)

void uiProgressBarSetValue(uiProgressBar *p, int n)
{
	// not on api.haiku-os.org
	p->pbar->SetTo(n);
}

uiProgressBar *uiNewProgressBar(void)
{
	uiProgressBar *p;

	uiHaikuNewControl(uiProgressBar, p);

	// layout constructor; not on api.haiku-os.org
	p->pbar = new BStatusBar(NULL, NULL, NULL);

	uiHaikuFinishNewControl(p, uiProgressBar);

	return p;
}
