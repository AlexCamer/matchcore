#include "mock/alloc.c"
#include "test.h"
#include "../src/pool.c"

#define POOL_ELEMENT_SIZE 29
#define POOL_ELEMENTS_PER_BLOCK 73
#define POOL_NUM_BLOCKS 5

TEST(Pool_addBlockTest) {
    struct Pool p;
    p.blocks = NULL;
    p.blockSize = 8;
    struct PoolStack *blocks[POOL_NUM_BLOCKS];
    for (usize i = 0; i < POOL_NUM_BLOCKS; i++) {
        Pool_addBlock(&p);
        blocks[i] = p.blocks;
    }
    struct PoolStack *block = p.blocks;
    for (isize i = POOL_NUM_BLOCKS - 1; i >= 0; i--) {
        ASSERT(block == blocks[i]);
        block = block->next;
    }
    ASSERT(blocks[0]->next == NULL);
    Pool_destroy(&p);
}

TEST(Pool_initTest) {
    struct Pool p;
    p.blocks = (struct PoolStack *) 1;
    p.freed = (struct PoolStack *) 1;
    p.offset = 0;
    p.elementSize = 0;
    p.blockSize = 0;
    Pool_init(&p, POOL_ELEMENT_SIZE, POOL_ELEMENTS_PER_BLOCK);
    ASSERT(p.blocks);
    ASSERT(p.blocks->next == NULL);
    ASSERT(p.freed == NULL);
    ASSERT(p.offset == sizeof(struct PoolStack));
    ASSERT(p.elementSize == POOL_ELEMENT_SIZE);
    ASSERT(p.blockSize == sizeof(struct PoolStack) + POOL_ELEMENT_SIZE * POOL_ELEMENTS_PER_BLOCK);
}

TEST(Pool_allocAddBlockTest) {
    struct Pool p;
    Pool_init(&p, POOL_ELEMENT_SIZE, POOL_ELEMENTS_PER_BLOCK);
    struct PoolStack *blocks[POOL_NUM_BLOCKS];
    for (usize i = 0; i < POOL_NUM_BLOCKS; i++) {
        for (int j = 0; j < POOL_ELEMENTS_PER_BLOCK; j++)
            Pool_alloc(&p);
        blocks[i] = p.blocks;
    }
    struct PoolStack *block = p.blocks;
    for (isize i = POOL_NUM_BLOCKS - 1; i >= 0; i--) {
        ASSERT(block == blocks[i]);
        block = block->next;
    }
    ASSERT(blocks[0]->next == NULL);
    Pool_destroy(&p);
}

TEST(Pool_allocTest) {
    struct Pool p;
    Pool_init(&p, POOL_ELEMENT_SIZE, POOL_ELEMENTS_PER_BLOCK);
    for (usize i = 0; i < POOL_NUM_BLOCKS; i++) {
        usize offset = sizeof(struct PoolStack);
        for (usize j = 0; j < POOL_ELEMENTS_PER_BLOCK; j++, offset += POOL_ELEMENT_SIZE)
            ASSERT(Pool_alloc(&p) == ((void *) p.blocks) + offset);
    }
    Pool_destroy(&p);
}

TEST(Pool_freeTest) {
    struct Pool p;
    Pool_init(&p, POOL_ELEMENT_SIZE, POOL_ELEMENTS_PER_BLOCK);
    void *ptrs[POOL_NUM_BLOCKS][POOL_ELEMENTS_PER_BLOCK];
    for (usize i = 0; i < POOL_NUM_BLOCKS; i++) {
        for (int j = 0; j < POOL_ELEMENTS_PER_BLOCK; j++)
            ptrs[i][j] = Pool_alloc(&p);
    }
    for (usize i = 0; i < POOL_ELEMENTS_PER_BLOCK; i++) {
        for (int j = 0; j < POOL_NUM_BLOCKS; j++)
            Pool_free(&p, ptrs[j][i]);
    }
    for (isize i = POOL_ELEMENTS_PER_BLOCK - 1; i >= 0; i--) {
        for (int j = POOL_NUM_BLOCKS - 1; j >= 0; j--)
            ASSERT(Pool_alloc(&p) == ptrs[j][i]);
    }
    ASSERT(p.freed == NULL);
    Pool_destroy(&p);
}

SUITE(Pool,
    Pool_addBlockTest,
    Pool_initTest,
    Pool_allocAddBlockTest,
    Pool_allocTest,
    Pool_freeTest
)
