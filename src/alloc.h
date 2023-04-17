#ifndef __ALLOC_H__
#define __ALLOC_H__

#include "types.h"

void *
safe_calloc(usize num, usize size);

void *
safe_malloc(usize size);

void *
safe_realloc(void *ptr, usize size);

void
safe_free(void *ptr);

#endif /* __ALLOC_H__ */
