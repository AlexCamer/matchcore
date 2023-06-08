#pragma once

#include "utility/types.h"

struct PoolStack;

struct Pool {
    struct PoolStack *blocks;
    struct PoolStack *freed;
    usize offset;
    usize elementSize;
    usize blockSize;
};

void Pool_Construct(struct Pool *pool, usize elementSize, usize elementsPerBlock);
void Pool_Destruct(struct Pool *pool);
void *Pool_Malloc(struct Pool *pool);
void Pool_Free(struct Pool *pool, void *ptr);
