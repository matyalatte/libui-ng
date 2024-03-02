// 18 november 2015
#include "uipriv_haiku.hpp"

struct uiMultilineEntry {
	uiHaikuControl c;
	BStringView *dummy;
	void (*onChanged)(uiMultilineEntry *, void *);
	void *onChangedData;
};

uiHaikuDefineControl(
	uiMultilineEntry,						// type name
	dummy								// handle
)

static void defaultOnChanged(uiMultilineEntry *e, void *data)
{
	// do nothing
}

char *uiMultilineEntryText(uiMultilineEntry *e)
{
	// TODO
	return NULL;
}

void uiMultilineEntrySetText(uiMultilineEntry *e, const char *text)
{
	// TODO
}

void uiMultilineEntryAppend(uiMultilineEntry *e, const char *text)
{
	// TODO
}

void uiMultilineEntryOnChanged(uiMultilineEntry *e, void (*f)(uiMultilineEntry *e, void *data), void *data)
{
	e->onChanged = f;
	e->onChangedData = data;
}

int uiMultilineEntryReadOnly(uiMultilineEntry *e)
{
	// TODO
	return 0;
}

void uiMultilineEntrySetReadOnly(uiMultilineEntry *e, int readonly)
{
	// TODO
}

uiMultilineEntry *uiNewMultilineEntry(void)
{
	uiMultilineEntry *e;

	uiHaikuNewControl(uiMultilineEntry, e);

	e->dummy = new BStringView(NULL, "TODO uiMultilineEntry not implemented");

	uiHaikuFinishNewControl(e, uiMultilineEntry);

	return e;
}
