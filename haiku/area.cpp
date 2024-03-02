// 18 november 2015
#include "uipriv_haiku.hpp"

// TODO scrollbars

class areaView : public BView {
public:
	// C++11! Inherit constructors.
	using BView::BView;

	uiArea *a;
	virtual void Draw(BRect updateRect);
};

struct uiArea {
	uiHaikuControl c;
	areaView *area;
	uiAreaHandler *ah;
};

uiHaikuDefineControl(
	uiArea,								// type name
	area									// handle
)

void areaView::Draw(BRect updateRect)
{
	uiAreaHandler *ah = this->a->ah;
	uiAreaDrawParams dp;
	BRect bounds;

	dp.Context = newContext(this);

	bounds = this->Bounds();
	dp.AreaWidth = bounds.right - bounds.left;
	dp.AreaHeight = bounds.bottom - bounds.top;

	dp.ClipX = updateRect.left;
	dp.ClipY = updateRect.top;
	dp.ClipWidth = updateRect.right - updateRect.left;
	dp.ClipHeight = updateRect.bottom - updateRect.top;

	// TODO scroll positions

	(*(ah->Draw))(ah, this->a, &dp);

	freeContext(dp.Context);
}

void uiAreaUpdateScroll(uiArea *a)
{
	// TODO
}

void uiAreaQueueRedrawAll(uiArea *a)
{
	// TODO does this really /queue/ a redraw? or does it redraw right away, regardless of the drawing machinery?
	a->area->Invalidate();
}

void uiAreaScrollTo(uiArea *a, double x, double y, double width, double height)
{
	// TODO
}

void uiAreaBeginUserWindowMove(uiArea *a)
{
	// TODO
}

void uiAreaBeginUserWindowResize(uiArea *a, uiWindowResizeEdge edge)
{
	// TODO
}

uiArea *uiNewArea(uiAreaHandler *ah)
{
	uiArea *a;

	uiHaikuNewControl(uiArea, a);

	a->ah = ah;

	// TODO:
	// - B_FULL_UPDATE_ON_RESIZE?
	// - B_FRAME_EVENTS?
	// - B_NAVIGABLE?
	// - B_SUBPIXEL_PRECISE?
	// - B_INVALIDATE_AFTER_LAYOUT?
	a->area = new areaView(NULL,
		B_WILL_DRAW | B_SUPPORTS_LAYOUT,
		NULL);
	a->area->a = a;
	// TODO background color

	// this is needed for alpha transparency (thanks mmu_man in irc.freenode.net/#haiku)
	// unfortunately TODO it kills the other compositing modes
	// (remember that if we ever drop Windows 7 we can use those)
	a->area->SetDrawingMode(B_OP_ALPHA);

	uiHaikuFinishNewControl(a, uiArea);

	return a;
}


uiArea *uiNewScrollingArea(uiAreaHandler *ah, int width, int height)
{
	// TODO
	return uiNewArea(ah);
}
