#include "test.h"
#include "../source/pool.c"

#define POOL_ELEMENT_SIZE 29
#define POOL_ELEMENTS_PER_BLOCK 73
#define POOL_NUM_BLOCKS 5

static struct Pool pool;

BEFORE(beforePoolTest) {
    Pool_init(&pool, POOL_ELEMENT_SIZE, POOL_ELEMENTS_PER_BLOCK);
}

AFTER(afterPoolTest) {
    Pool_destroy(&pool);
}

TEST(addBlockTest) {
    Pool_destroy(&pool);
    pool.blocks = NULL;
    pool.blockSize = 8;
    struct PoolStack *blocks[POOL_NUM_BLOCKS];
    for (usize i = 0; i < POOL_NUM_BLOCKS; i++) {
        addBlock(&pool);
        blocks[i] = pool.blocks;
    }
    struct PoolStack *block = pool.blocks;
    for (isize i = POOL_NUM_BLOCKS - 1; i >= 0; i--) {
        ASSERT(block == blocks[i]);
        block = block->next;
    }
    ASSERT(blocks[0]->next == NULL);
}

TEST(Pool_initTest) {
    Pool_destroy(&pool);
    pool.blocks = (struct PoolStack *) 1;
    pool.freed = (struct PoolStack *) 1;
    pool.offset = 0;
    pool.elementSize = 0;
    pool.blockSize = 0;
    Pool_init(&pool, POOL_ELEMENT_SIZE, POOL_ELEMENTS_PER_BLOCK);
    ASSERT(pool.blocks);
    ASSERT(pool.blocks->next == NULL);
    ASSERT(pool.freed == NULL);
    ASSERT(pool.offset == sizeof(struct PoolStack));
    ASSERT(pool.elementSize == POOL_ELEMENT_SIZE);
    ASSERT(pool.blockSize == sizeof(struct PoolStack) + POOL_ELEMENT_SIZE
                                                      * POOL_ELEMENTS_PER_BLOCK);
}

TEST(Pool_allocAddBlockTest) {
    struct PoolStack *blocks[POOL_NUM_BLOCKS];
    for (usize i = 0; i < POOL_NUM_BLOCKS; i++) {
        for (int j = 0; j < POOL_ELEMENTS_PER_BLOCK; j++)
            Pool_alloc(&pool);
        blocks[i] = pool.blocks;
    }
    struct PoolStack *block = pool.blocks;
    for (isize i = POOL_NUM_BLOCKS - 1; i >= 0; i--) {
        ASSERT(block == blocks[i]);
        block = block->next;
    }
    ASSERT(blocks[0]->next == NULL);
}

TEST(Pool_allocTest) {
    for (usize i = 0; i < POOL_NUM_BLOCKS; i++) {
        usize offset = sizeof(struct PoolStack);
        for (usize j = 0; j < POOL_ELEMENTS_PER_BLOCK; j++, offset += POOL_ELEMENT_SIZE)
            ASSERT(Pool_alloc(&pool) == ((void *) pool.blocks) + offset);
    }
}

TEST(Pool_freeTest) {
    void *ptrs[POOL_NUM_BLOCKS][POOL_ELEMENTS_PER_BLOCK];
    for (usize i = 0; i < POOL_NUM_BLOCKS; i++) {
        for (int j = 0; j < POOL_ELEMENTS_PER_BLOCK; j++)
            ptrs[i][j] = Pool_alloc(&pool);
    }
    for (usize i = 0; i < POOL_ELEMENTS_PER_BLOCK; i++) {
        for (int j = 0; j < POOL_NUM_BLOCKS; j++)
            Pool_free(&pool, ptrs[j][i]);
    }
    for (isize i = POOL_ELEMENTS_PER_BLOCK - 1; i >= 0; i--) {
        for (int j = POOL_NUM_BLOCKS - 1; j >= 0; j--)
            ASSERT(Pool_alloc(&pool) == ptrs[j][i]);
    }
    ASSERT(pool.freed == NULL);
}

SUITE("Pool", beforePoolTest, afterPoolTest,
    addBlockTest,
    Pool_initTest,
    Pool_allocAddBlockTest,
    Pool_allocTest,
    Pool_freeTest
)
