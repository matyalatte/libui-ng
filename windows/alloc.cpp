// 4 december 2014
#include "uipriv_windows.hpp"

typedef std::vector<uint8_t> byteArray;

static std::map<uint8_t *, byteArray *> heap;
static std::map<byteArray *, const char *> types;

void initAlloc(void)
{
	// do nothing
}

void uninitAlloc(void)
{
	if (heap.size() == 0)
		return;

	char *out;
	out = new char[1];
	out[0] = 0;

	for (const auto &alloc : heap) {
		int len;
		char *new_out;
		len = snprintf(NULL, 0, "%s%p %s\n", out, (void*)(alloc.first), types[alloc.second]);
		new_out = new char[len + 1];
		new_out[len] = 0;
		snprintf(new_out, len + 1, "%s%p %s\n", out, (void*)(alloc.first), types[alloc.second]);
		delete[] out;
		out = new_out;
	}
	uiprivUserBug("Some data was leaked; either you left a uiControl lying around or there's a bug in libui itself. Leaked data:\n%s", out);
	delete[] out;
}

#define rawBytes(pa) (&((*pa)[0]))

void *uiprivAlloc(size_t size, const char *type)
{
	byteArray *out;

	out = new byteArray(size, 0);
	heap[rawBytes(out)] = out;
	types[out] = type;
	return rawBytes(out);
}

void *uiprivRealloc(void *_p, size_t size, const char *type)
{
	uint8_t *p = (uint8_t *) _p;
	byteArray *arr;

	if (p == NULL)
		return uiprivAlloc(size, type);
	arr = heap[p];
	// TODO does this fill in?
	arr->resize(size, 0);
	heap.erase(p);
	heap[rawBytes(arr)] = arr;
	return rawBytes(arr);
}

void uiprivFree(void *_p)
{
	uint8_t *p = (uint8_t *) _p;

	if (p == NULL)
		uiprivImplBug("attempt to uiprivFree(NULL)");
	types.erase(heap[p]);
	delete heap[p];
	heap.erase(p);
}
