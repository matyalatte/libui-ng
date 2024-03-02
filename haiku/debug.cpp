#include <cstdio>
#include "uipriv_haiku.hpp"

// LONGTERM don't halt on release builds

void uiprivRealBug(const char *file, const char *line, const char *func, const char *prefix, const char *format, va_list ap)
{
	// TODO
	fprintf(stderr, "[libui] %s:%s:%s() %s", file, line, func, prefix);
}
