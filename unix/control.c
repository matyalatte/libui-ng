// 16 august 2015
#include "uipriv_unix.h"

void uiUnixControlSetContainer(uiUnixControl *c, GtkContainer *container, gboolean remove)
{
	(*(c->SetContainer))(c, container, remove);
}

#define uiUnixControlSignature 0x556E6978

uiUnixControl *uiUnixAllocControl(size_t n, uint32_t typesig, const char *typenamestr)
{
	return uiUnixControl(uiAllocControl(n, uiUnixControlSignature, typesig, typenamestr));
}

uiControl *uiControlParent(uiControl *c)
{
	if (uiControlToplevel(c))
		return NULL;
	return uiUnixControl(c)->parent;
}

void uiControlSetParent(uiControl *c, uiControl *parent)
{
	if (uiControlToplevel(c))
		uiUserBugCannotSetParentOnToplevel("uiWindow");
	uiControlVerifySetParent(c, parent);
	uiUnixControl(c)->parent = parent;
}

int uiControlVisible(uiControl *c)
{
	return gtk_widget_get_visible((GtkWidget *)uiControlHandle(c));
}

void uiControlShow(uiControl *c)
{
	if (uiControlToplevel(c)) {
		uiprivWindowShow(c);
		return;
	}
	// TODO part of massive hack about hidden before
	uiUnixControl(c)->explicitlyHidden=FALSE;
	gtk_widget_show((GtkWidget *)uiControlHandle(c));
}

void uiControlHide(uiControl *c)
{
	// TODO part of massive hack about hidden before
	uiUnixControl(c)->explicitlyHidden=TRUE;
	gtk_widget_hide((GtkWidget *)uiControlHandle(c));
}

int uiControlEnabled(uiControl *c)
{
	return gtk_widget_get_sensitive((GtkWidget *)uiControlHandle(c));
}

void uiControlEnable(uiControl *c)
{
	gtk_widget_set_sensitive((GtkWidget *)uiControlHandle(c), TRUE);
}

void uiControlDisable(uiControl *c)
{
	gtk_widget_set_sensitive((GtkWidget *)uiControlHandle(c), FALSE);
}
