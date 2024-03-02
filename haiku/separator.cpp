// 18 november 2015
#include "uipriv_haiku.hpp"

struct uiSeparator {
	uiHaikuControl c;
	BStringView *dummy;
};

uiHaikuDefineControl(
	uiSeparator,						// type name
	dummy								// handle
)

uiSeparator *uiNewHorizontalSeparator(void)
{
	uiSeparator *s;

	uiHaikuNewControl(uiSeparator, s);

	s->dummy = new BStringView(NULL, "TODO uiSeparator not implemented");

	uiHaikuFinishNewControl(s, uiSeparator);

	return s;
}
