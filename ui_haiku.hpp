// 17 november 2015

/*
This file assumes that you have included the various Haiku header files and "ui.h" beforehand. It provides API-specific functions for interfacing with foreign controls in Haiku.
*/

#ifndef __LIBUI_UI_HAIKU_HPP__
#define __LIBUI_UI_WAIKU_HPP__

#ifndef __cplusplus
#error Sorry; ui_haiku.hpp can currently only be used from C++ code.
#endif

extern "C" {

typedef struct uiHaikuControl uiHaikuControl;
struct uiHaikuControl {
	uiControl c;
	uiControl *parent;
};
#define uiHaikuControl(this) ((uiHaikuControl *) (this))

#define uiHaikuControlDefaultParent(type) \
	static uiControl *type ## Parent(uiControl *c) \
	{ \
		return uiHaikuControl(c)->parent; \
	}
#define uiHaikuControlDefaultSetParent(type) \
	static void type ## SetParent(uiControl *c, uiControl *parent) \
	{ \
		uiControlVerifySetParent(c, parent); \
		uiHaikuControl(c)->parent = parent; \
	}
#define uiHaikuControlDefaultToplevel(type) \
	static int type ## Toplevel(uiControl *c) \
	{ \
		/* TODO */ return 0; \
	}
#define uiHaikuControlDefaultVisible(type) \
	static int type ## Visible(uiControl *c) \
	{ \
		/* TODO */ return 0; \
	}
#define uiHaikuControlDefaultShow(type) \
	static void type ## Show(uiControl *c) \
	{ \
		/* TODO */ \
	}
#define uiHaikuControlDefaultHide(type) \
	static void type ## Hide(uiControl *c) \
	{ \
		/* TODO */ \
	}
#define uiHaikuControlDefaultEnabled(type) \
	static int type ## Enabled(uiControl *c) \
	{ \
		/* TODO */ return 0; \
	}
#define uiHaikuControlDefaultEnable(type) \
	static void type ## Enable(uiControl *c) \
	{ \
		/* TODO */ \
	}
#define uiHaikuControlDefaultDisable(type) \
	static void type ## Disable(uiControl *c) \
	{ \
		/* TODO */ \
	}

// TODO document
#define uiHaikuDefineControlWithOnDestroy(type, handlefield, onDestroy) \
	static void _ ## type ## Destroy(uiControl *c) \
	{ \
		type *hthis = type(c); \
		onDestroy; \
		delete hthis->handlefield; \
	} \
	static uintptr_t _ ## type ## Handle(uiControl *c) \
	{ \
		return (uintptr_t) (type(c)->handlefield); \
	} \
	uiHaikuControlDefaultParent(type) \
	uiHaikuControlDefaultSetParent(type) \
	uiHaikuControlDefaultToplevel(type) \
	uiHaikuControlDefaultVisible(type) \
	uiHaikuControlDefaultShow(type) \
	uiHaikuControlDefaultHide(type) \
	uiHaikuControlDefaultEnabled(type) \
	uiHaikuControlDefaultEnable(type) \
	uiHaikuControlDefaultDisable(type)

#define uiHaikuDefineControl(type, handlefield) \
	uiHaikuDefineControlWithOnDestroy(type, handlefield, (void) hthis;)

#define uiHaikuNewControl(type, var) \
	var = type(uiHaikuAllocControl(sizeof (type), type ## Signature, #type)); \
	uiControl(var)->Destroy = type ## Destroy; \
	uiControl(var)->Handle = type ## Handle; \
	uiControl(var)->Parent = type ## Parent; \
	uiControl(var)->SetParent = type ## SetParent; \
	uiControl(var)->Toplevel = type ## Toplevel; \
	uiControl(var)->Visible = type ## Visible; \
	uiControl(var)->Show = type ## Show; \
	uiControl(var)->Hide = type ## Hide; \
	uiControl(var)->Enabled = type ## Enabled; \
	uiControl(var)->Enable = type ## Enable; \
	uiControl(var)->Disable = type ## Disable; \
_UI_EXTERN uiHaikuControl *uiHaikuAllocControl(size_t n, uint32_t typesig, const char *typenamestr);

#define uiHaikuFinishNewControl(variable, type) \
	uiHaikuFinishControl(uiControl(variable));

// This is a function used to set up a control.
// Don't call it directly; use uiHaikuFinishNewControl() instead.
_UI_EXTERN void uiHaikuFinishControl(uiControl *c);

// TODO document this
_UI_EXTERN void uiHaikuRegisterEventHandler(uint32 what, void (*handler)(BMessage *));

// uiHaikuStrdupText() takes the given string and produces a copy of it suitable for being freed by uiFreeText().
_UI_EXTERN char *uiHaikuStrdupText(const char *);

}

#endif
