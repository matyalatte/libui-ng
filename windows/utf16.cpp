// 21 april 2016
#include "uipriv_windows.hpp"

// see http://stackoverflow.com/a/29556509/3408572

wchar_t *toUTF16(const char *str)
{
	WCHAR *wstr;
	WCHAR *wp;
	size_t n;
	uint32_t rune;

	if (*str == '\0')			// empty string
		return emptyUTF16();
	n = uiprivUTF8UTF16Count(str, 0);
	wstr = (WCHAR *) uiprivAlloc((n + 1) * sizeof (WCHAR), "WCHAR[]");
	wp = wstr;
	while (*str) {
		str = uiprivUTF8DecodeRune(str, 0, &rune);
		n = uiprivUTF16EncodeRune(rune, wp);
		wp += n;
	}
	return wstr;
}

char *toUTF8(const wchar_t *wstr)
{
	char *str;
	char *sp;
	size_t n;
	uint32_t rune;

	if (*wstr == L'\0')		// empty string
		return emptyUTF8();
	n = uiprivUTF16UTF8Count(wstr, 0);
	str = (char *) uiprivAlloc((n + 1) * sizeof (char), "char[]");
	sp = str;
	while (*wstr) {
		wstr = uiprivUTF16DecodeRune(wstr, 0, &rune);
		n = uiprivUTF8EncodeRune(rune, sp);
		sp += n;
	}
	return str;
}

#define MBTWC(str, str_size, wstr, wstr_size) MultiByteToWideChar(CP_UTF8, 0, str, str_size, wstr, wstr_size)

wchar_t *toUTF16Faster(const char *str)
{
	WCHAR *wstr;
	WCHAR *wp;
	size_t n;

	if (*str == '\0')			// empty string
		return emptyUTF16();
	n = uiprivUTF8UTF16CountFaster(str);
	wstr = (WCHAR *) uiprivAlloc((n + 1) * sizeof (WCHAR), "WCHAR[]");
	wp = wstr;
	while (*str) {
		str = uiprivUTF8UTF16Faster(str, wp, &n);
		wp += n;
	}
	return wstr;
}

#define WCTMB(wstr, wstr_size, str, str_size) WideCharToMultiByte(CP_UTF8, 0, wstr, wstr_size, str, str_size, NULL, NULL)

char *toUTF8Faster(const wchar_t *wstr)
{
	char *str;
	char *sp;
	size_t n;

	if (*wstr == L'\0')		// empty string
		return emptyUTF8();
	n = uiprivUTF16UTF8CountFaster(wstr);
	str = (char *) uiprivAlloc((n + 1) * sizeof (char), "char[]");
	sp = str;
	while (*wstr) {
		wstr = uiprivUTF16UTF8Faster(wstr, sp, &n);
		sp += n;
	}
	return str;
}

#define MBTWC(str, wstr, bufsiz) MultiByteToWideChar(CP_UTF8, 0, str, -1, wstr, bufsiz)

wchar_t *toUTF16Old(const char *str)
{
	WCHAR *wstr;
	int n;

	if (*str == '\0')			// empty string
		return emptyUTF16();
	n = MBTWC(str, NULL, 0);
	if (n == 0) {
		logLastError(L"error figuring out number of characters to convert to");
		return emptyUTF16();
	}
	wstr = (WCHAR *) uiprivAlloc(n * sizeof (WCHAR), "WCHAR[]");
	if (MBTWC(str, wstr, n) != n) {
		logLastError(L"error converting from UTF-8 to UTF-16");
		// and return an empty string
		*wstr = L'\0';
	}
	return wstr;
}

#define WCTMB(wstr, str, bufsiz) WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, bufsiz, NULL, NULL)

char *toUTF8Old(const wchar_t *wstr)
{
	char *str;
	int n;

	if (*wstr == L'\0')		// empty string
		return emptyUTF8();
	n = WCTMB(wstr, NULL, 0);
	if (n == 0) {
		logLastError(L"error figuring out number of characters to convert to");
		return emptyUTF8();
	}
	str = (char *) uiprivAlloc(n * sizeof (char), "char[]");
	if (WCTMB(wstr, str, n) != n) {
		logLastError(L"error converting from UTF-16 to UTF-8");
		// and return an empty string
		*str = '\0';
	}
	return str;
}


WCHAR *utf16dup(const WCHAR *orig)
{
	WCHAR *out;
	size_t len;

	len = wcslen(orig);
	out = (WCHAR *) uiprivAlloc((len + 1) * sizeof (WCHAR), "WCHAR[]");
	wcscpy_s(out, len + 1, orig);
	return out;
}

WCHAR *strf(const WCHAR *format, ...)
{
	va_list ap;
	WCHAR *str;

	va_start(ap, format);
	str = vstrf(format, ap);
	va_end(ap);
	return str;
}

WCHAR *vstrf(const WCHAR *format, va_list ap)
{
	va_list ap2;
	WCHAR *buf;
	size_t n;

	if (*format == L'\0')
		return emptyUTF16();

	va_copy(ap2, ap);
	n = _vscwprintf(format, ap2);
	va_end(ap2);
	n++;		// terminating L'\0'

	buf = (WCHAR *) uiprivAlloc(n * sizeof (WCHAR), "WCHAR[]");
	// includes terminating L'\0' according to example in https://msdn.microsoft.com/en-us/library/xa1a1a6z.aspx
	vswprintf_s(buf, n, format, ap);

	return buf;
}

// TODO merge the following two with the toUTF*()s?

// Let's shove these utility routines here too.
// Prerequisite: lfonly is UTF-8.
char *LFtoCRLF(const char *lfonly)
{
	char *crlf;
	size_t i, len;
	char *out;

	len = strlen(lfonly);
	crlf = (char *) uiprivAlloc((len * 2 + 1) * sizeof (char), "char[]");
	out = crlf;
	for (i = 0; i < len; i++) {
		if (*lfonly == '\n')
			*crlf++ = '\r';
		*crlf++ = *lfonly++;
	}
	*crlf = '\0';
	return out;
}

// Prerequisite: s is UTF-8.
void CRLFtoLF(char *s)
{
	char *t = s;

	for (; *s != '\0'; s++) {
		// be sure to preserve \rs that are genuinely there
		if (*s == '\r' && *(s + 1) == '\n')
			continue;
		*t++ = *s;
	}
	*t = '\0';
	// pad out the rest of t, just to be safe
	while (t != s)
		*t++ = '\0';
}

// std::to_string() always uses %f; we want %g
// fortunately std::iostream seems to use %g by default so
WCHAR *ftoutf16(double d)
{
	std::wostringstream ss;
	std::wstring s;

	ss << d;
	s = ss.str();		// to be safe
	return utf16dup(s.c_str());
}

// to complement the above
WCHAR *itoutf16(int i)
{
	std::wostringstream ss;
	std::wstring s;

	ss << i;
	s = ss.str();		// to be safe
	return utf16dup(s.c_str());
}
