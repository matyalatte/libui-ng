// 14 april 2016
#include "uipriv_haiku.hpp"

struct uiFontButton {
	uiHaikuControl c;
	BStringView *dummy;
};

uiHaikuDefineControl(
	uiFontButton,						// type name
	uiFontButtonType,					// type function
	dummy								// handle
)

void uiFontButtonFont(uiFontButton *b, uiFontDescriptor *desc)
{
	// TODO
}

void uiFontButtonOnChanged(uiFontButton *b, void (*f)(uiFontButton *, void *), void *data)
{
	// TODO
}

uiFontButton *uiNewFontButton(void)
{
	uiFontButton *b;

	b = (uiFontButton *) uiNewControl(uiFontButtonType());

	b->dummy = new BStringView(NULL, "TODO uiFontButton not implemented");

	uiHaikuFinishNewControl(b, uiFontButton);

	return b;
}

void uiFreeFontButtonFont(uiFontDescriptor *desc)
{
	// TODO
}
