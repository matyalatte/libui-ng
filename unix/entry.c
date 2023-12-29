// 11 june 2015
#include "uipriv_unix.h"

struct uiEntry {
	uiUnixControl c;
	GtkWidget *widget;
	GtkEntry *entry;
	GtkEditable *editable;
	void (*onChanged)(uiEntry *, void *);
	void *onChangedData;
	gulong onChangedSignal;
	void (*onFilesDropped)(uiEntry *, int, char**, void *);
	void *onFilesDroppedData;
	guint onFilesDroppedTime;
};

uiUnixControlAllDefaults(uiEntry)

static void onChanged(GtkEditable *editable, gpointer data)
{
	uiEntry *e = uiEntry(data);

	(*(e->onChanged))(e, e->onChangedData);
}

static void onFilesDropped(GtkEditable *editable,
							GdkDragContext *context,
							gint x,
							gint y,
							GtkSelectionData *data,
							guint info,
							guint time,
							gpointer user_data)
{
	uiEntry *e = uiEntry(user_data);

	if (gtk_selection_data_get_length(data) == 0 ||
		gtk_selection_data_get_format(data) != 8 ||
		(e->onFilesDroppedTime == time && time != 0)) {
		gtk_drag_finish(context, FALSE, FALSE, time);
		return;
	}

	// Need to check the 'time' parameter cause callbacks can be called twice.
	e->onFilesDroppedTime = time;

	gchar **uris;
	gchar **files;
	gchar *hostname;
	GError *error = NULL;
	gint i;

	uris = gtk_selection_data_get_uris(data);
	if (uris == NULL) {
		gtk_drag_finish(context, FALSE, FALSE, time);
		return;
	}

	for (i = 0; uris[i] != NULL; i++) ;
	files = g_new0(gchar *, i + 1);
	int count = i;

	for (i = 0; uris[i] != NULL; i++) {
		files[i] = g_filename_from_uri(uris[i], &hostname, &error);
		if (files[i] == NULL) {
			g_clear_error(&error);
			g_free(hostname);
			g_strfreev(files);
			g_strfreev(uris);
			gtk_drag_finish(context, FALSE, FALSE, time);
			return;
		}
		g_free(hostname);
	}
	g_strfreev(uris);

	(*(e->onFilesDropped))(e, count, files, e->onFilesDroppedData);
	g_strfreev(files);

	gtk_drag_finish(context, TRUE, TRUE, time);
}

static void defaultOnChanged(uiEntry *e, void *data)
{
	// do nothing
}

static void defaultOnFilesDropped(uiEntry *e, int count, char** names, void *data)
{
	// do nothing
}

char *uiEntryText(uiEntry *e)
{
	return uiUnixStrdupText(gtk_entry_get_text(e->entry));
}

void uiEntrySetText(uiEntry *e, const char *text)
{
	// we need to inhibit sending of ::changed because this WILL send a ::changed otherwise
	g_signal_handler_block(e->editable, e->onChangedSignal);
	gtk_entry_set_text(e->entry, text);
	g_signal_handler_unblock(e->editable, e->onChangedSignal);
	// don't queue the control for resize; entry sizes are independent of their contents
}

void uiEntryOnChanged(uiEntry *e, void (*f)(uiEntry *, void *), void *data)
{
	e->onChanged = f;
	e->onChangedData = data;
}

void uiEntryOnFilesDropped(uiEntry *e, void (*f)(uiEntry *, int, char**, void *), void *data)
{
	e->onFilesDropped = f;
	e->onFilesDroppedData = data;
}

int uiEntryReadOnly(uiEntry *e)
{
	return gtk_editable_get_editable(e->editable) == FALSE;
}

void uiEntrySetReadOnly(uiEntry *e, int readonly)
{
	gboolean editable;

	editable = TRUE;
	if (readonly)
		editable = FALSE;
	gtk_editable_set_editable(e->editable, editable);
}

int uiEntryAcceptDrops(uiEntry *e)
{
	return gtk_drag_dest_get_target_list(e->widget) == NULL;
}

GtkTargetEntry drop_targets[] = { { "text/uri-list", 0, 10 } };

void uiEntrySetAcceptDrops(uiEntry *e, int accept)
{
	if (accept) {
		gtk_drag_dest_set(e->widget,
						7,
						drop_targets,
						sizeof(drop_targets) / sizeof(drop_targets[0]),
						GDK_ACTION_COPY);
	} else {
		gtk_drag_dest_set(e->widget, 7, NULL, 0, 0);
	}
}

char *uiEntryPlaceholder(uiEntry *e)
{
	return uiUnixStrdupText(gtk_entry_get_placeholder_text(e->entry));
}

void uiEntrySetPlaceholder(uiEntry *e, const char *text)
{
	gtk_entry_set_placeholder_text(e->entry, text);
}

static uiEntry *finishNewEntry(GtkWidget *w, const gchar *signal)
{
	uiEntry *e;

	uiUnixNewControl(uiEntry, e);

	e->widget = w;
	e->entry = GTK_ENTRY(e->widget);
	e->editable = GTK_EDITABLE(e->widget);

	e->onChangedSignal = g_signal_connect(e->widget, signal, G_CALLBACK(onChanged), e);
	g_signal_connect(e->widget, "drag-data-received", G_CALLBACK(onFilesDropped), e);
	uiEntryOnChanged(e, defaultOnChanged, NULL);
	uiEntryOnFilesDropped(e, defaultOnFilesDropped, NULL);

	return e;
}

uiEntry *uiNewEntry(void)
{
	return finishNewEntry(gtk_entry_new(), "changed");
}

uiEntry *uiNewPasswordEntry(void)
{
	GtkWidget *e;

	e = gtk_entry_new();
	gtk_entry_set_visibility(GTK_ENTRY(e), FALSE);
	return finishNewEntry(e, "changed");
}

// TODO make it use a separate function to be type-safe
uiEntry *uiNewSearchEntry(void)
{
	return finishNewEntry(gtk_search_entry_new(), "search-changed");
}
