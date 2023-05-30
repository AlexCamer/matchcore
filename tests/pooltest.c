#include "test.h"
#include "pool.c"

#define POOL_ELEMENT_SIZE 29
#define POOL_ELEMENTS_PER_BLOCK 73
#define POOL_NUM_BLOCKS 5

TEST(Pool_AddBlockTest) {
    struct Pool pool;
    pool.blocks = NULL;
    pool.blockSize = 8;
    struct PoolStack *blocks[POOL_NUM_BLOCKS];
    for (usize i = 0; i < POOL_NUM_BLOCKS; i++) {
        Pool_AddBlock(&pool);
        blocks[i] = pool.blocks;
    }
    struct PoolStack *block = pool.blocks;
    for (isize i = POOL_NUM_BLOCKS - 1; i >= 0; i--) {
        ASSERT(block == blocks[i]);
        block = block->next;
    }
    ASSERT(blocks[0]->next == NULL);
    Pool_Destruct(&pool);
}

TEST(Pool_ConstructTest) {
    struct Pool pool;
    pool.blocks = (struct PoolStack *) 1;
    pool.freed = (struct PoolStack *) 1;
    pool.offset = 0;
    pool.elementSize = 0;
    pool.blockSize = 0;
    Pool_Construct(&pool, POOL_ELEMENT_SIZE, POOL_ELEMENTS_PER_BLOCK);
    ASSERT(pool.blocks);
    ASSERT(pool.blocks->next == NULL);
    ASSERT(pool.freed == NULL);
    ASSERT(pool.offset == sizeof(struct PoolStack));
    ASSERT(pool.elementSize == POOL_ELEMENT_SIZE);
    ASSERT(pool.blockSize == sizeof(struct PoolStack) + POOL_ELEMENT_SIZE
                                                      * POOL_ELEMENTS_PER_BLOCK);
    Pool_Destruct(&pool);
}

TEST(Pool_MallocTest) {
    struct Pool pool;
    Pool_Construct(&pool, POOL_ELEMENT_SIZE, POOL_ELEMENTS_PER_BLOCK);
    for (usize i = 0; i < POOL_NUM_BLOCKS; i++) {
        usize offset = sizeof(struct PoolStack);
        for (usize j = 0; j < POOL_ELEMENTS_PER_BLOCK; j++, offset += POOL_ELEMENT_SIZE)
            ASSERT(Pool_Malloc(&pool) == ((void *) pool.blocks) + offset);
    }
    Pool_Destruct(&pool);
}

TEST(Pool_FreeTest) {
    struct Pool pool;
    Pool_Construct(&pool, POOL_ELEMENT_SIZE, POOL_ELEMENTS_PER_BLOCK);
    void *ptrs[POOL_NUM_BLOCKS][POOL_ELEMENTS_PER_BLOCK];
    for (usize i = 0; i < POOL_NUM_BLOCKS; i++) {
        for (int j = 0; j < POOL_ELEMENTS_PER_BLOCK; j++)
            ptrs[i][j] = Pool_Malloc(&pool);
    }
    for (usize i = 0; i < POOL_ELEMENTS_PER_BLOCK; i++) {
        for (int j = 0; j < POOL_NUM_BLOCKS; j++)
            Pool_Free(&pool, ptrs[j][i]);
    }
    for (isize i = POOL_ELEMENTS_PER_BLOCK - 1; i >= 0; i--) {
        for (int j = POOL_NUM_BLOCKS - 1; j >= 0; j--)
            ASSERT(Pool_Malloc(&pool) == ptrs[j][i]);
    }
    ASSERT(pool.freed == NULL);
    Pool_Destruct(&pool);
}

SUITE(Pool_AddBlockTest,
      Pool_ConstructTest,
      Pool_MallocTest,
      Pool_FreeTest)
