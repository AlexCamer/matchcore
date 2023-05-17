#include <stdlib.h>
#include "alloc.h"
#include "macros.h"
#include "pool.h"

struct PoolStack {
    struct PoolStack *next;
};

void Pool_addBlock(struct Pool *p) {
    struct PoolStack *block = (struct PoolStack *) xmalloc(p->blockSize);
    block->next = p->blocks;
    p->blocks = block;
}

void Pool_init(struct Pool *p, usize elementSize, usize elementsPerBlock) {
    p->blocks = NULL;
    p->freed = NULL;
    p->offset = sizeof(struct PoolStack);
    p->elementSize = MAX(elementSize, sizeof(struct PoolStack));
    p->blockSize = sizeof(struct PoolStack) + p->elementSize * elementsPerBlock;
    Pool_addBlock(p);
}

void Pool_destroy(struct Pool *p) {
    do {
        struct PoolStack *block = p->blocks;
        p->blocks = p->blocks->next;
        free(block);
    } while (p->blocks);
}

void *Pool_alloc(struct Pool *p) {
    if (p->freed) {
        void *ptr = (void *) p->freed;
        p->freed = p->freed->next;
        return ptr;
    }
    if (p->offset == p->blockSize) {
        p->offset = sizeof(struct PoolStack);
        Pool_addBlock(p);
    }
    void *ptr = ((void *) p->blocks) + p->offset;
    p->offset += p->elementSize;
    return ptr;
}

void Pool_free(struct Pool *p, void *ptr) {
    struct PoolStack *freed = (struct PoolStack *) ptr;
    freed->next = p->freed;
    p->freed = freed;
}
