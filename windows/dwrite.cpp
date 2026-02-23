// 14 april 2016
#include "uipriv_windows.hpp"
#include "attrstr.hpp"

IDWriteFactory *dwfactory = NULL;
IDWriteTextFormat *dwDefaultTextFormat = NULL;

static bool supportColorEmoji = 0;
static bool useLegacyRenderer = 0;

int uiWindowsUseLegacyRenderer()
{
	return static_cast<int>(useLegacyRenderer);
}

void uiWindowsSetUseLegacyRenderer(int use_legacy)
{
	if (supportColorEmoji)
		useLegacyRenderer = use_legacy != 0;
}

// IID for IDWriteFactory2
// {0439FC60-CA44-4994-8DEE-3A9AF7B732EC}
static const IID IID_IDWriteFactory2 =
{ 0x0439FC60, 0xCA44, 0x4994,{ 0x8D,0xEE,0x3A,0x9A,0xF7,0xB7,0x32,0xEC } };

// Check if D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT is supported or not.
static bool testColorEmoji()
{
	if (!dwfactory) {
		return false;
	}

	IUnknown* factory2 = nullptr;
	HRESULT hr = dwfactory->QueryInterface(IID_IDWriteFactory2, (void**)&factory2);

	if (SUCCEEDED(hr)) {
		factory2->Release();
		return true;
	} else {
		return false;
	}
}

// TOOD rename to something else, maybe
HRESULT uiprivInitDrawText(void)
{
	// TOOD use DWRITE_FACTORY_TYPE_ISOLATED instead?
	HRESULT hr;
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
		__uuidof (IDWriteFactory),
		(IUnknown **) (&dwfactory));
	supportColorEmoji = testColorEmoji();
	if (supportColorEmoji) {
		LOGFONT lfont;
		GetObject(hMessageFont, sizeof(LOGFONT), &lfont);
		hr = dwfactory->CreateTextFormat(
			lfont.lfFaceName,
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			(FLOAT)abs(lfont.lfHeight),
			L"",
			&dwDefaultTextFormat
		);
		if (FAILED(hr))
			supportColorEmoji = false;
	}
	if (!supportColorEmoji)
		useLegacyRenderer = true;
	return hr;
}

void uiprivUninitDrawText(void)
{
	dwfactory->Release();
	if (dwDefaultTextFormat)
		dwDefaultTextFormat->Release();
}

fontCollection *uiprivLoadFontCollection(void)
{
	fontCollection *fc;
	HRESULT hr;

	fc = uiprivNew(fontCollection);
	// always get the latest available font information
	hr = dwfactory->GetSystemFontCollection(&(fc->fonts), TRUE);
	if (hr != S_OK)
		logHRESULT(L"error getting system font collection", hr);
	fc->userLocaleSuccess = GetUserDefaultLocaleName(fc->userLocale, LOCALE_NAME_MAX_LENGTH);
	return fc;
}

void uiprivFontCollectionFree(fontCollection *fc)
{
	fc->fonts->Release();
	uiprivFree(fc);
}

WCHAR *uiprivFontCollectionFamilyName(fontCollection *fc, IDWriteFontFamily *family)
{
	IDWriteLocalizedStrings *names;
	WCHAR *str;
	HRESULT hr;

	hr = family->GetFamilyNames(&names);
	if (hr != S_OK)
		logHRESULT(L"error getting names of font out", hr);
	str = uiprivFontCollectionCorrectString(fc, names);
	names->Release();
	return str;
}

WCHAR *uiprivFontCollectionCorrectString(fontCollection *fc, IDWriteLocalizedStrings *names)
{
	UINT32 index;
	BOOL exists;
	UINT32 length;
	WCHAR *wname;
	HRESULT hr;

	// this is complex, but we ignore failure conditions to allow fallbacks
	// 1) If the user locale name was successfully retrieved, try it
	// 2) If the user locale name was not successfully retrieved, or that locale's string does not exist, or an error occurred, try L"en-us", the US English locale
	// 3) And if that fails, assume the first one
	// This algorithm is straight from MSDN: https://msdn.microsoft.com/en-us/library/windows/desktop/dd368214%28v=vs.85%29.aspx
	// For step 2 to work, start by setting hr to S_OK and exists to FALSE.
	// TODO does it skip step 2 entirely if step 1 fails? rewrite it to be a more pure conversion of the MSDN code?
	hr = S_OK;
	exists = FALSE;
	if (fc->userLocaleSuccess != 0)
		hr = names->FindLocaleName(fc->userLocale, &index, &exists);
	if (hr != S_OK || (hr == S_OK && !exists))
		hr = names->FindLocaleName(L"en-us", &index, &exists);
	// TODO check hr again here? or did I decide that would be redundant because COM requires output arguments to be filled regardless of return value?
	if (!exists)
		index = 0;

	hr = names->GetStringLength(index, &length);
	if (hr != S_OK)
		logHRESULT(L"error getting length of font name", hr);
	// GetStringLength() does not include the null terminator, but GetString() does
	wname = (WCHAR *) uiprivAlloc((length + 1) * sizeof (WCHAR), "WCHAR[]");
	hr = names->GetString(index, wname, length + 1);
	if (hr != S_OK)
		logHRESULT(L"error getting font name", hr);

	return wname;
}

void uiprivDrawTextToControl(uiControl *c, HDC hdc, RECT *rect, const WCHAR *wtext, COLORREF color, BOOL centerize) {
	D2D1_RECT_F layoutRect;
	ID2D1DCRenderTarget *rt;
	ID2D1SolidColorBrush* brush;
	HRESULT hr;

	// We use a render target of uiWindow for child controls.
	uiControl *top = uiControlGetToplevel(c);
	if (!top)
		return;
	rt = uiprivGetWindowRenderTarget(uiWindow(top));
	if (!rt)
		return;

	rt->CreateSolidColorBrush(
		D2D1::ColorF(
			GetRValue(color) / 255.0f,
			GetGValue(color) / 255.0f,
			GetBValue(color) / 255.0f
		),
		&brush
	);

	rt->BindDC(hdc, rect);
	rt->BeginDraw();

	layoutRect = D2D1::RectF(
		(FLOAT)rect->left,
		(FLOAT)rect->top,
		(FLOAT)rect->right,
		(FLOAT)rect->bottom
	);

	if (centerize) {
		dwDefaultTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	} else {
		dwDefaultTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	}
	dwDefaultTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	dwDefaultTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

	rt->DrawTextW(
		wtext,
		(UINT32)wcslen(wtext),
		dwDefaultTextFormat,
		layoutRect,
		brush,
		// draw color emojis
		D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT,
		// use the GDI's metrics
		DWRITE_MEASURING_MODE_GDI_CLASSIC
	);

	hr = rt->EndDraw();
	if (hr == D2DERR_RECREATE_TARGET)
		uiprivReleaseWindowRenderTarget(uiWindow(top));
	brush->Release();
}
