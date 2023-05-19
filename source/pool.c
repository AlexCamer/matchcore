#include <stdlib.h>
#include "alloc.h"
#include "macros.h"
#include "pool.h"

struct PoolStack {
    struct PoolStack *next;
};

void Pool_addBlock(struct Pool *pool) {
    struct PoolStack *block = (struct PoolStack *) xmalloc(pool->blockSize);
    block->next = pool->blocks;
    pool->blocks = block;
}

void Pool_init(struct Pool *pool, usize elementSize, usize elementsPerBlock) {
    pool->blocks = NULL;
    pool->freed = NULL;
    pool->offset = sizeof(struct PoolStack);
    pool->elementSize = MAX(elementSize, sizeof(struct PoolStack));
    pool->blockSize = sizeof(struct PoolStack) + pool->elementSize * elementsPerBlock;
    Pool_addBlock(pool);
}

void Pool_destroy(struct Pool *pool) {
    do {
        struct PoolStack *block = pool->blocks;
        pool->blocks = pool->blocks->next;
        free(block);
    } while (pool->blocks);
}

void *Pool_alloc(struct Pool *pool) {
    if (pool->freed) {
        void *ptr = (void *) pool->freed;
        pool->freed = pool->freed->next;
        return ptr;
    }
    if (pool->offset == pool->blockSize) {
        pool->offset = sizeof(struct PoolStack);
        Pool_addBlock(pool);
    }
    void *ptr = ((void *) pool->blocks) + pool->offset;
    pool->offset += pool->elementSize;
    return ptr;
}

void Pool_free(struct Pool *pool, void *ptr) {
    struct PoolStack *freed = (struct PoolStack *) ptr;
    freed->next = pool->freed;
    pool->freed = freed;
}
