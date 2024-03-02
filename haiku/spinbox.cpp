// 18 november 2015
#include "uipriv_haiku.hpp"

struct uiSpinbox {
	uiHaikuControl c;
	BStringView *dummy;
};

uiHaikuDefineControl(
	uiSpinbox,							// type name
	dummy								// handle
)

int uiSpinboxValue(uiSpinbox *s)
{
	// TODO
	return 0;
}

void uiSpinboxSetValue(uiSpinbox *s, int value)
{
	// TODO
}

void uiSpinboxOnChanged(uiSpinbox *s, void (*f)(uiSpinbox *s, void *data), void *data)
{
	// TODO
}

uiSpinbox *uiNewSpinbox(int min, int max)
{
	uiSpinbox *s;

	uiHaikuNewControl(uiSpinbox, s);

	s->dummy = new BStringView(NULL, "TODO uiSpinbox not implemented");

	uiHaikuFinishNewControl(s, uiSpinbox);

	return s;
}
