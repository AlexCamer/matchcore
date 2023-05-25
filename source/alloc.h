#pragma once

#include <stdlib.h>
#include "../include/types.h"

void *xmalloc(usize size);
void *xcalloc(usize size, usize elementSize);
void *xrealloc(void *ptr, usize size);
void xfree(void *ptr);
