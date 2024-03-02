#include "uipriv_haiku.hpp"

struct uiGrid {
	uiHaikuControl c;
	BStringView *dummy;
};

uiHaikuDefineControl(
	uiGrid,								// type name
	uiGridType,							// type function
	dummy								// handle
)

void uiGridAppend(uiGrid *g, uiControl *c, int left, int top, int xspan, int yspan, int hexpand, uiAlign halign, int vexpand, uiAlign valign)
{
	// TODO
}

void uiGridInsertAt(uiGrid *g, uiControl *c, uiControl *existing, uiAt at, int xspan, int yspan, int hexpand, uiAlign halign, int vexpand, uiAlign valign)
{
	// TODO
}

int uiGridPadded(uiGrid *g)
{
	// TODO
	return 0;
}

void uiGridSetPadded(uiGrid *g, int padded)
{
	// TODO
}

uiGrid *uiNewGrid(const char *title)
{
	uiGrid *g;

	g = (uiGrid *) uiNewControl(uiGridType());

	g->dummy = new BStringView(NULL, "TODO uiGrid not implemented");

	uiHaikuFinishNewControl(g, uiGrid);

	return g;
}
