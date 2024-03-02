#include "uipriv_haiku.hpp"

struct uiForm {
	uiHaikuControl c;
	BStringView *dummy;
};

uiHaikuDefineControl(
	uiForm,								// type name
	dummy								// handle
)

void uiFormAppend(uiForm *f, const char *label, uiControl *c, int stretchy)
{
	// TODO
}

void uiFormDelete(uiForm *f, int index)
{
	// TODO
}

int uiFormNumChildren(uiForm *f)
{
	// TODO
	return 0;
}

int uiFormPadded(uiForm *f)
{
	// TODO
	return 0;
}

void uiFormSetPadded(uiForm *f, int padded)
{
	// TODO
}

uiForm *uiNewForm(const char *title)
{
	uiForm *f;

	uiHaikuNewControl(uiForm, f);

	f->dummy = new BStringView(NULL, "TODO uiForm not implemented");

	uiHaikuFinishNewControl(f, uiForm);

	return f;
}
