// 18 november 2015
#include "uipriv_haiku.hpp"

struct uiCombobox {
	uiHaikuControl c;
	BStringView *dummy;
};

uiHaikuDefineControl(
	uiCombobox,							// type name
	dummy								// handle
)

void uiComboboxAppend(uiCombobox *c, const char *text)
{
	// TODO
}

void uiComboboxInsertAt(uiCombobox *c, int n, const char *text)
{
	// TODO
}

void uiComboboxDelete(uiCombobox *c, int n)
{
	// TODO
}

void uiComboboxClear(uiCombobox *c)
{
	// TODO
}

int uiComboboxNumItems(uiCombobox *c)
{
	// TODO
	return 0;
}

int uiComboboxSelected(uiCombobox *c)
{
	// TODO
	// return 0 so the area test can work
	return 0;
}

void uiComboboxSetSelected(uiCombobox *c, int n)
{
	// TODO
}

void uiComboboxOnSelected(uiCombobox *c, void (*f)(uiCombobox *c, void *data), void *data)
{
	// TODO
}

static uiCombobox *finishNewCombobox(void)
{
	uiCombobox *c;

	uiHaikuNewControl(uiCombobox, c);

	c->dummy = new BStringView(NULL, "TODO uiCombobox not implemented");

	uiHaikuFinishNewControl(c, uiCombobox);

	return c;
}

uiCombobox *uiNewCombobox(void)
{
	return finishNewCombobox();
}
