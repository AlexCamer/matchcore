#include <stdio.h>
#include <stdlib.h>
#include "../../include/utility/alloc.h"

static inline void OutOfMemory(void) {
    fprintf(stderr, "Out of memory.\n");
    exit(1);
}

void *Malloc(usize size) {
    void *ptr = malloc(size);
    if (ptr == NULL)
        OutOfMemory();
    return ptr;
}

void *Calloc(usize size, usize elementSize) {
    void *ptr = calloc(size, elementSize);
    if (ptr == NULL)
        OutOfMemory();
    return ptr;
}

void *Realloc(void *ptr, usize size) {
    ptr = realloc(ptr, size);
    if (ptr == NULL)
        OutOfMemory();
    return ptr;
}

void Free(void *ptr) {
    free(ptr);
}
