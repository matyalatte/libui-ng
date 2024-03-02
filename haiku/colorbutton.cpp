// 15 may 2016
#include "uipriv_haiku.hpp"

struct uiColorButton {
	uiHaikuControl c;
	BStringView *dummy;
};

uiHaikuDefineControl(
	uiColorButton,						// type name
	uiColorButtonType,					// type function
	dummy								// handle
)

void uiColorButtonColor(uiColorButton *b, double *r, double *g, double *bl, double *a)
{
	// TODO
}

void uiColorButtonSetColor(uiColorButton *b, double r, double g, double bl, double a)
{
	// TODO
}

void uiColorButtonOnChanged(uiColorButton *b, void (*f)(uiColorButton *, void *), void *data)
{
	// TODO
}

uiColorButton *uiNewColorButton(void)
{
	uiColorButton *b;

	b = (uiColorButton *) uiNewControl(uiColorButtonType());

	b->dummy = new BStringView(NULL, "TODO uiColorButton not implemented");

	uiHaikuFinishNewControl(b, uiColorButton);

	return b;
}
