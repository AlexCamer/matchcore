#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "types.h"

void *
safe_calloc(usize num,
            usize size);

void *
safe_malloc(usize size);

void *
safe_realloc(void * ptr,
             usize size);

#define safe_free(ptr) free(ptr)

#endif /* __MEMORY_H__ */
