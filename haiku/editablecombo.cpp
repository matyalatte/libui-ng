#include "uipriv_haiku.hpp"

struct uiEditableCombobox {
	uiHaikuControl c;
	BStringView *dummy;
};

uiHaikuDefineControl(
	uiEditableCombobox,					// type name
	dummy								// handle
)

void uiEditableComboboxAppend(uiEditableCombobox *c, const char *text)
{
	// TODO
}

char *uiEditableComboboxText(uiEditableCombobox *c)
{
	// TODO
	return NULL;
}

void uiEditableComboboxSetText(uiEditableCombobox *c, const char *text)
{
	// TODO
}

void uiEditableComboboxOnChanged(uiEditableCombobox *c, void (*f)(uiEditableCombobox *c, void *data), void *data)
{
	// TODO
}

uiEditableCombobox *uiNewEditableCombobox(void)
{
	uiEditableCombobox *c;

	uiHaikuNewControl(uiEditableCombobox, c);

	c = (uiEditableCombobox *) uiNewControl(uiEditableComboboxType());

	c->dummy = new BStringView(NULL, "TODO uiEditableCombobox not implemented");

	uiHaikuFinishNewControl(c, uiEditableCombobox);

	return c;
}
