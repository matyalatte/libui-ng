#include "../ui.h"
#include "uipriv.h"

#include "config.h"

const char *uiVersion(void)
{
	return UI_VERSION;
}

int *uiVersionYear(void)
{
	return UI_VERSION_YEAR;
}

int *uiVersionMonth(void)
{
	return UI_VERSION_MONTH;
}

int *uiVersionDay(void)
{
	return UI_VERSION_DAY;
}

int *uiVersionHash(void)
{
	return UI_VERSION_HASH;
}

int *uiVersionNumber(void)
{
	return UI_VERSION_YEAR * 10000 + UI_VERSION_MONTH * 100 + UI_VERSION_DAY;
}
