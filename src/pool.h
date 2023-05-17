#pragma once

#include "types.h"

struct PoolStack;

struct Pool {
    struct PoolStack *blocks;
    struct PoolStack *freed;
    usize offset;
    usize elementSize;
    usize blockSize;
};

void Pool_init(struct Pool *p, usize elementSize, usize elementsPerBlock);
void Pool_destroy(struct Pool *p);
void *Pool_alloc(struct Pool *p);
void Pool_free(struct Pool *p, void *ptr);
