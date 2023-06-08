#include "../../include/utility/alloc.h"
#include "../../include/utility/macros.h"
#include "../../include/utility/pool.h"

struct PoolStack {
    struct PoolStack *next;
};

static inline void Pool_AddBlock(struct Pool *pool) {
    struct PoolStack *block = (struct PoolStack *) Malloc(pool->blockSize);
    block->next = pool->blocks;
    pool->blocks = block;
    pool->offset = sizeof(struct PoolStack);
}

void Pool_Construct(struct Pool *pool, usize elementSize, usize elementsPerBlock) {
    pool->blocks = NULL;
    pool->freed = NULL;
    pool->elementSize = MAX(elementSize, sizeof(struct PoolStack));
    pool->blockSize = sizeof(struct PoolStack) + pool->elementSize * elementsPerBlock;
    Pool_AddBlock(pool);
}

void Pool_Destruct(struct Pool *pool) {
    do {
        struct PoolStack *block = pool->blocks;
        pool->blocks = block->next;
        Free(block);
    } while (pool->blocks != NULL);
}

void *Pool_Malloc(struct Pool *pool) {
    if (pool->freed != NULL) {
        struct PoolStack *ptr = pool->freed;
        pool->freed = ptr->next;
        return (void *) ptr;
    }
    if (pool->offset == pool->blockSize)
        Pool_AddBlock(pool);
    void *ptr = ((void *) pool->blocks) + pool->offset;
    pool->offset += pool->elementSize;
    return ptr;
}

void Pool_Free(struct Pool *pool, void *ptr) {
    struct PoolStack *freed = (struct PoolStack *) ptr;
    freed->next = pool->freed;
    pool->freed = freed;
}
