#include <stddef.h>

static unsigned char heap[65536];
static size_t heap_index = 0;

void *malloc(size_t size)
{
    if (heap_index + size > sizeof(heap))
        return 0;

    void *ptr = &heap[heap_index];
    heap_index += size;
    return ptr;
}

void free(void *ptr)
{
    (void)ptr;
}

void *memset(void *ptr, int value, size_t num)
{
    unsigned char *p = ptr;
    for (size_t i = 0; i < num; i++)
        p[i] = value;
    return ptr;
}