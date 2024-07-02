// 11 june 2015
#include "uipriv_unix.h"

struct uiLabel {
	uiUnixControl c;
	GtkWidget *widget;
	GtkMisc *misc;
	GtkLabel *label;
};

uiUnixControlAllDefaults(uiLabel)

char *uiLabelText(uiLabel *l)
{
	return uiUnixStrdupText(gtk_label_get_text(l->label));
}

void uiLabelSetText(uiLabel *l, const char *text)
{
	gtk_label_set_text(l->label, text);
}

void uiLabelSetTextColor(uiLabel *l, double r, double g, double b)
{
	guint16 red = (guint16)(r * 0xFFFF);
	guint16 green = (guint16)(g * 0xFFFF);
	guint16 blue = (guint16)(b * 0xFFFF);
	PangoAttribute *attr = pango_attr_foreground_new(red, green, blue);

	GtkLabel *label = GTK_LABEL(l->widget);

	PangoAttrList *attrs = gtk_label_get_attributes(GTK_LABEL(label));

	if (attrs == NULL)
		attrs = pango_attr_list_new();

	pango_attr_list_change(attrs, attr);
	gtk_label_set_attributes(GTK_LABEL(label), attrs);
	pango_attr_list_unref(attrs);
}

uiLabel *uiNewLabel(const char *text)
{
	uiLabel *l;

	uiUnixNewControl(uiLabel, l);

	l->widget = gtk_label_new(text);
	l->misc = GTK_MISC(l->widget);
	l->label = GTK_LABEL(l->widget);

	gtk_misc_set_alignment(l->misc, 0, 0);

	return l;
}
