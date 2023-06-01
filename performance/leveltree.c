#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../source/alloc.h"
#include "../source/level.h"
#include "../source/levelhashheap.h"
#include "../source/macros.h"
#include "../source/order.h"

#define SEED 52387301
#define CAPACITY EXP2(6)
#define INSERTS EXP2(24)

i32 main(void) {
    srand(SEED);
    struct LevelTree *trees[CAPACITY];
    for (usize i = 0; i < CAPACITY; i++) {
        trees[i] = Malloc(sizeof(struct LevelTree));
        LevelTree_Construct(trees[i]);
    }
    struct Order order = { 0 };
    clock_t start = clock();
    for (usize i = 0; i < INSERTS; i++) {
        order.base.orderID = i;
        LevelTree_GetOrAdd(trees[rand() % CAPACITY], rand() % MUL_EXP2(LEVEL_HASH_HEAP_CAPACITY, 1));
        // Level_Add(level, &order);
        if ((i % 3) == 0) {
            struct LevelTree *tree = trees[rand() % CAPACITY];
            if (LevelTree_Empty(tree))
                continue;
            LevelTree_Remove(tree, LevelTree_Peek(tree));
        }
    }
    clock_t time = clock() - start;
    i32 milliseconds = time * 1000 / CLOCKS_PER_SEC;
    printf("Time %d\n", milliseconds);
    return 0;
}
