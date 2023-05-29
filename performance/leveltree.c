#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../source/alloc.h"
#include "../source/level.h"
#include "../source/levelhashheap.h"

#define SEED 52387301
#define CAPACITY EXP2(8)
#define INSERTS EXP2(24)

i32 main(void) {
    srand(SEED);
    struct LevelTree *trees[CAPACITY];
    for (usize i = 0; i < CAPACITY; i++) {
        trees[i] = Malloc(sizeof(struct LevelTree));
        LevelTree_Construct(trees[i]);
    }
    clock_t start = clock();
    for (usize i = 0; i < INSERTS; i++)
            LevelTree_GetOrAdd(trees[rand() % CAPACITY], rand() % LEVEL_HASH_HEAP_CAPACITY);
    clock_t time = clock() - start;
    i32 milliseconds = time * 1000 / CLOCKS_PER_SEC;
    printf("Time %d\n", milliseconds);
    return 0;
}
