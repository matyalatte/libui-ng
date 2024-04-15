#include <stdio.h>
#include "unit.h"
#include <time.h>

int unitWindowOnClosingQuit(uiWindow *w, void *data)
{
	uiQuit();
	return 1;
}

int unitTestsSetup(void **state)
{
	*state = malloc(sizeof(struct state));
	assert_non_null(*state);
	return 0;
}

int unitTestsTeardown(void **state)
{
	free(*state);
	return 0;
}

int unitTestSetup(void **_state)
{
	struct state *state = *_state;
	uiInitOptions o = {0};

	assert_null(uiInit(&o));
	state->w = uiNewWindow("Unit Test", UNIT_TEST_WINDOW_WIDTH, UNIT_TEST_WINDOW_HEIGHT, 0);
	uiWindowOnClosing(state->w, unitWindowOnClosingQuit, NULL);
	return 0;
}

int unitTestTeardown(void **_state)
{
	struct state *state = *_state;

	uiWindowSetChild(state->w, uiControl(state->c));
	uiControlShow(uiControl(state->w));
	//uiMain();
	uiMainSteps();
	uiMainStep(1);
	uiControlDestroy(uiControl(state->w));
	uiUninit();
	return 0;
}

struct unitTest {
	int (*fn)(void);
};

void test_utf_func(wchar_t*(*UTF16)(const char*), char*(*UTF8)(const wchar_t*),
				   const char* test_str, int trials) {
	clock_t start, end;
	start = clock();
	for (int i = 0; i < trials; i++) {
		wchar_t *wstr = UTF16(test_str);
		char *new_str = UTF8(wstr);
		uiprivFree(wstr);
		uiprivFree(new_str);
	}
	end = clock();
	printf("%f ", (double)(end - start) * 1000000.0 / trials);
}

void print_str_as_hex(const char *str) {
	while(*str) {
		printf("%x ", (int)(*str & 0xFF));
		str++;
	}
	printf("\n");
}

void compare_utf_func(const char *base_str, int char_size) {
	/*
	// check if toUTF*Faster works fine.
	print_str_as_hex(base_str);
	wchar_t *wstr = toUTF16Faster(base_str);
	char *new_str = toUTF8Faster(wstr);
	//print_str_as_hex(new_str);
	uiprivFree(wstr);
	uiprivFree(new_str);
	*/

	char test_str[841];
	char *pstr = test_str;
	test_str[0] = 0;
	int trials = 2000000;
	for (int i = 0; i <= 200; i += 10) {
		printf("%d ", i);
		test_utf_func(toUTF16, toUTF8, test_str, trials);
		test_utf_func(toUTF16Faster, toUTF8Faster, test_str, trials);
		test_utf_func(toUTF16Old, toUTF8Old, test_str, trials);
		printf("\n");
		memcpy(pstr, base_str, char_size * 10);
		pstr += char_size * 10;
		*pstr = 0;
	}
	printf("\n\n");
}

/* The output is data for gnuplot (https://hostcat.fhsu.edu/cdclark/static/apps/gnuplot/)
set multiplot layout 2,2

set xrange [0:200]
set yrange [0:5000]
set xlabel "word count"
set ylabel "ms/1M calls"
set title "Ascii"

plot "data.txt" index 0 using 1:2 with lines title "utf.h",\
"data.txt" index 0 using 1:3 with lines title "proposed",\
"data.txt" index 0 using 1:4 with lines title "winapi"

set xlabel " "
set ylabel " "

set title "2-byte characters"
plot "data.txt" index 1 using 1:2 with lines title "utf.h",\
"data.txt" index 1 using 1:3 with lines title "proposed",\
"data.txt" index 1 using 1:4 with lines title "winapi"

set title "3-byte characters"
plot "data.txt" index 2 using 1:2 with lines title "utf.h",\
"data.txt" index 2 using 1:3 with lines title "proposed",\
"data.txt" index 2 using 1:4 with lines title "winapi"

set title "4-byte characters"
plot "data.txt" index 3 using 1:2 with lines title "utf.h",\
"data.txt" index 3 using 1:3 with lines title "proposed",\
"data.txt" index 3 using 1:4 with lines title "winapi"

unset multiplot
*/

int main(void)
{
	const char* base_str = "abcdefghij";
	const char* base_str2 = "\u0630\u0631\u0632\u0633\u0634\u0635\u0636\u0637\u0638\u0639";
	const char* base_str3 = "\u3041\u3042\u3043\u3044\u3045\u3046\u3047\u3048\u3049\u3050";
	const char* base_str4 = "\U0001F604\U0001F605\U0001F606\U0001F607\U0001F608\U0001F609\U0001F610\U0001F611\U0001F612\U0001F613";

	compare_utf_func(base_str, 1);
	compare_utf_func(base_str2, 2);
	compare_utf_func(base_str3, 3);
	compare_utf_func(base_str4, 4);

	return 0;
}
