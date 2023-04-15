#include "memory.h"
#include "stdio.h"
#include "stdlib.h"

static inline
void
safe_alloc_oom(void)
{
    fprintf(stderr, "Out of memory.\n");
    fflush(stderr);
    abort();
}

void *
safe_calloc(usize num,
            usize size)
{
    void * ptr = calloc(num, size);
    if (!ptr)
        safe_alloc_oom();
    return ptr;
}

void *
safe_malloc(usize size)
{
    void * ptr = malloc(size);
    if (!ptr)
        safe_alloc_oom();
    return ptr;
}

void *
safe_realloc(void * ptr,
             usize size)
{
    ptr = realloc(ptr, size);
    if (!ptr)
        safe_alloc_oom();
    return ptr;
}
