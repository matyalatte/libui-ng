// 18 november 2015
#include "uipriv_haiku.hpp"

struct uiDateTimePicker {
	uiHaikuControl c;
	BStringView *dummy;
};

uiHaikuDefineControl(
	uiDateTimePicker,						// type name
	dummy								// handle
)

static uiDateTimePicker *finishNewDateTimePicker(void)
{
	uiDateTimePicker *d;

	uiHaikuNewControl(uiDateTimePicker, d);

	d->dummy = new BStringView(NULL, "TODO uiDateTimePicker not implemented");

	uiHaikuFinishNewControl(d, uiDateTimePicker);

	return d;
}

uiDateTimePicker *uiNewDateTimePicker(void)
{
	return finishNewDateTimePicker();
}

uiDateTimePicker *uiNewDatePicker(void)
{
	return finishNewDateTimePicker();
}

uiDateTimePicker *uiNewTimePicker(void)
{
	return finishNewDateTimePicker();
}
