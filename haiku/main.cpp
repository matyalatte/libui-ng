// 17 november 2015
#include "uipriv_haiku.hpp"

static BApplication *app;

uiInitOptions options;

const char *uiInit(uiInitOptions *o)
{
	status_t err;

	options = *o;
	// TODO properly set the MIME type
	// TODO andlabs-libui?
	app = new BApplication("application/x-vnd.andlabs.libui", &err);
	if (err != B_NO_ERROR) {
		delete app;
		// TODO
		return "fail";
	}
	initAlloc();
	return NULL;
}

void uiUninit(void)
{
	delete app;
	uninitAlloc();
}

void uiFreeInitError(const char *err)
{
	// TODO
}

void uiMain(void)
{
	app->Run();
}

void uiQuit(void)
{
	// TODO app->PostMessage(B_QUIT_REQUESTED);?
	// TODO see window.cpp for why that alone won't work
	app->Quit();
}

void uiQueueMain(void (*f)(void *data), void *data)
{
	// TODO
}
