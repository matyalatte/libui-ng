// 26 june 2016
#include "uipriv_haiku.hpp"
#include "table.hpp"

struct uiTable {
	uiHaikuControl c;
	BStringView *dummy;
};

uiHaikuDefineControl(
	uiTable,							// type name
	dummy								// handle
)

uiSortIndicator uiTableHeaderSortIndicator(uiTable *t, int lcol)
{
	// TODO
	return uiSortIndicatorNone;
}

void uiTableHeaderSetSortIndicator(uiTable *t, int lcol, uiSortIndicator indicator)
{
	// TODO
}

void uiTableHeaderOnClicked(uiTable *t, void (*f)(uiTable *, int, void *), void *data)
{
	// TODO
}

void uiTableOnSelectionChanged(uiTable *t, void (*f)(uiTable *t, void *data), void *data)
{
	// TODO
}

uiTableSelection* uiTableGetSelection(uiTable *t)
{
	// TODO
	return NULL;
}

void uiTableSetSelection(uiTable *t, uiTableSelection *sel)
{
	// TODO
}

void uiTableAppendTextColumn(uiTable *t, const char *name, int textModelColumn, int textEditableModelColumn, uiTableTextColumnOptionalParams *textParams)
{
	// TODO
}

void uiTableAppendImageColumn(uiTable *t, const char *name, int imageModelColumn)
{
	// TODO
}

void uiTableAppendImageTextColumn(uiTable *t, const char *name, int imageModelColumn, int textModelColumn, int textEditableModelColumn, uiTableTextColumnOptionalParams *textParams)
{
	// TODO
}

void uiTableAppendCheckboxColumn(uiTable *t, const char *name, int checkboxModelColumn, int checkboxEditableModelColumn)
{
	// TODO
}

void uiTableAppendCheckboxTextColumn(uiTable *t, const char *name, int checkboxModelColumn, int checkboxEditableModelColumn, int textModelColumn, int textEditableModelColumn, uiTableTextColumnOptionalParams *textParams)
{
	// TODO
}

void uiTableAppendProgressBarColumn(uiTable *t, const char *name, int progressModelColumn)
{
	// TODO
}

void uiTableAppendButtonColumn(uiTable *t, const char *name, int buttonModelColumn, int buttonClickableModelColumn)
{
	// TODO
}

int uiTableHeaderVisible(uiTable *t)
{
	// TODO
	return 0;
}

void uiTableHeaderSetVisible(uiTable *t, int visible)
{
	// TODO
}

void uiTableOnRowClicked(uiTable *t, void (*f)(uiTable *, int, void *), void *data)
{
	// TODO
}

void uiTableOnRowDoubleClicked(uiTable *t, void (*f)(uiTable *, int, void *), void *data)
{
	// TODO
}

uiTable *uiNewTable(uiTableParams *p)
{
	uiTable *t;

	uiHaikuNewControl(uiTable, t);

	t->dummy = new BStringView(NULL, "TODO uiTable not implemented");

	uiHaikuFinishNewControl(t, uiTable);

	return t;
}

int uiTableColumnWidth(uiTable *t, int column)
{
	// TODO
	return 0;
}

void uiTableColumnSetWidth(uiTable *t, int column, int width)
{
	// TODO
}

uiTableSelectionMode uiTableGetSelectionMode(uiTable *t)
{
	// TODO
	return uiTableSelectionModeNone;
}

void uiTableSetSelectionMode(uiTable *t, uiTableSelectionMode mode)
{
	// TODO
}
