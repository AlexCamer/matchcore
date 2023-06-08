#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/utility/alloc.h"
#include "../include/engine/level.h"
#include "../include/engine/levelhashheap.h"
#include "../include/utility/macros.h"
#include "../include/engine/order.h"

#define SEED 52387301
#define CAPACITY EXP2(6)
#define INSERTS EXP2(24)
#define RANGE MUL_EXP2(LEVEL_HASH_HEAP_CAPACITY, 3)

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
        LevelTree_GetOrAdd(trees[rand() % CAPACITY], rand() % RANGE);
        // Level_Add(level, &order);
        if ((i % 8) == 0) {
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
