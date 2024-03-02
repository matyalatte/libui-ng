// 19 november 2015
#include "uipriv_haiku.hpp"
#include "draw.hpp"

uiDrawContext *newContext(BView *view)
{
	uiDrawContext *c;

	c = uiNew(uiDrawContext);
	c->view = view;
	return c;
}

void freeContext(uiDrawContext *c)
{
	uiFree(c);
}

// TODO verify this
static void setHighColor(BView *view, uiDrawBrush *b)
{
	view->SetHighColor(b->R * 255,
		b->G * 255,
		b->B * 255,
		b->A * 255);
}

// TODO path ended checks; error checks

void uiDrawStroke(uiDrawContext *c, uiDrawPath *path, uiDrawBrush *b, uiDrawStrokeParams *p)
{
	cap_mode cap;
	join_mode join;

	switch (p->Cap) {
	case uiDrawLineCapFlat:
		cap = B_BUTT_CAP;
		break;
	case uiDrawLineCapRound:
		cap = B_ROUND_CAP;
		break;
	case uiDrawLineCapSquare:
		cap = B_SQUARE_CAP;
		break;
	}
	switch (p->Join) {
	case uiDrawLineJoinMiter:
		join = B_MITER_JOIN;
		break;
	case uiDrawLineJoinRound:
		join = B_ROUND_JOIN;
		break;
	case uiDrawLineJoinBevel:
		join = B_BEVEL_JOIN;
		break;
	}
	c->view->SetLineMode(cap, join, p->MiterLimit);
	c->view->SetPenSize(p->Thickness);
	// TODO line dashes
	switch (b->Type) {
	case uiDrawBrushTypeSolid:
		setHighColor(c->view, b);
		c->view->StrokeShape(path->shape);
		break;
	case uiDrawBrushTypeLinearGradient:
		// TODO
	case uiDrawBrushTypeRadialGradient:
		// TODO
		break;
//	case uiDrawBrushTypeImage:
	}
}

void uiDrawFill(uiDrawContext *c, uiDrawPath *path, uiDrawBrush *b)
{
	// TODO not documented on api.haiku-os.org
	switch (path->fillMode) {
	case uiDrawFillModeWinding:
		c->view->SetFillRule(B_NONZERO);
		break;
	case uiDrawFillModeAlternate:
		c->view->SetFillRule(B_EVEN_ODD);
		break;
	}
	switch (b->Type) {
	case uiDrawBrushTypeSolid:
		setHighColor(c->view, b);
		c->view->FillShape(path->shape);
		break;
	case uiDrawBrushTypeLinearGradient:
		// TODO
	case uiDrawBrushTypeRadialGradient:
		// TODO
		break;
//	case uiDrawBrushTypeImage:
	}
}

// TODO not documented on api.haiku-os.org
void uiDrawClip(uiDrawContext *c, uiDrawPath *path)
{
	c->view->ClipToShape(path->shape);
}

void uiDrawSave(uiDrawContext *c)
{
	c->view->PushState();
}

void uiDrawRestore(uiDrawContext *c)
{
	c->view->PopState();
}
