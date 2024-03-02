#include "uipriv_haiku.hpp"
#include "draw.hpp"

// TODO none of this is documented on api.haiku-os.org

static void m2a(uiDrawMatrix *m, BAffineTransform *a)
{
	a->sx = m->M11;
	a->shy = m->M12;
	a->shx = m->M21;
	a->sy = m->M22;
	a->tx = m->M31;
	a->ty = m->M32;
}

static void a2m(BAffineTransform *a, uiDrawMatrix *m)
{
	m->M11 = a->sx;
	m->M12 = a->shy;
	m->M21 = a->shx;
	m->M22 = a->sy;
	m->M31 = a->tx;
	m->M32 = a->ty;
}

void uiDrawMatrixSetIdentity(uiDrawMatrix *m)
{
	setIdentity(m);
}

void uiDrawMatrixTranslate(uiDrawMatrix *m, double x, double y)
{
	BAffineTransform a;

	m2a(m, &a);
	a.TranslateBy(x, y);
	a2m(&a, m);
}

void uiDrawMatrixScale(uiDrawMatrix *m, double xCenter, double yCenter, double x, double y)
{
	BAffineTransform a;

	m2a(m, &a);
	a.ScaleBy(BPoint(xCenter, yCenter), BPoint(x, y));
	a2m(&a, m);
}

void uiDrawMatrixRotate(uiDrawMatrix *m, double x, double y, double amount)
{
	BAffineTransform a;

	m2a(m, &a);
	// TODO degrees or radians?
	a.RotateBy(BPoint(x, y), amount);
	a2m(&a, m);
}

void uiDrawMatrixSkew(uiDrawMatrix *m, double x, double y, double xamount, double yamount)
{
	BAffineTransform a;

	m2a(m, &a);
	// TODO degrees or radians?
	a.ShearBy(BPoint(x, y), BPoint(xamount, yamount));
	a2m(&a, m);
}

void uiDrawMatrixMultiply(uiDrawMatrix *dest, uiDrawMatrix *src)
{
	BAffineTransform c;
	BAffineTransform d;

	m2a(dest, &c);
	m2a(src, &d);
	c.Multiply(d);
	a2m(&c, dest);
}

int uiDrawMatrixInvertible(uiDrawMatrix *m)
{
	// TODO
	return 0;
}

int uiDrawMatrixInvert(uiDrawMatrix *m)
{
	// TODO
	return 0;
}

void uiDrawMatrixTransformPoint(uiDrawMatrix *m, double *x, double *y)
{
	// TODO
}

void uiDrawMatrixTransformSize(uiDrawMatrix *m, double *x, double *y)
{
	// TODO
}

void uiDrawTransform(uiDrawContext *c, uiDrawMatrix *m)
{
	BAffineTransform a;

	m2a(m, &a);
	// see windows/draw.c
	a.Multiply(c->view->Transform());
	c->view->SetTransform(a);
}
