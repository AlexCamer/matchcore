#include "alloc.h"
#include <stdio.h>
#include <stdlib.h>

static inline void xallocOOM(void) {
    fprintf(stderr, "Out of memory.\n");
    exit(1);
}

void *xmalloc(usize size) {
    void *ptr = malloc(size);
    if (!ptr)
        xallocOOM();
    return ptr;
}

void *xcalloc(usize size, usize elementSize) {
    void *ptr = calloc(size, elementSize);
    if (!ptr)
        xallocOOM();
    return ptr;
}

void *xrealloc(void *ptr, usize size) {
    ptr = realloc(ptr, size);
    if (!ptr)
        xallocOOM();
    return ptr;
}

void xfree(void *ptr) {
    free(ptr);
}
