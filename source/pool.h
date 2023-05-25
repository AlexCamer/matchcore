#pragma once

#include "../include/types.h"

struct PoolStack;

struct Pool {
    struct PoolStack *blocks;
    struct PoolStack *freed;
    usize offset;
    usize elementSize;
    usize blockSize;
};

void Pool_construct(struct Pool *pool, usize elementSize, usize elementsPerBlock);
void Pool_destruct(struct Pool *pool);
void *Pool_malloc(struct Pool *pool);
void Pool_free(struct Pool *pool, void *ptr);
