#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../source/alloc.h"
#include "../source/level.h"
#include "../source/levelhashheap.h"
#include "../source/macros.h"
#include "../source/order.h"

#define SEED 52387301
#define CAPACITY EXP2(8)
#define INSERTS EXP2(24)
#define RANGE 4096

i32 main(void) {
    srand(SEED);
    struct LevelBucket *buckets[CAPACITY];
    for (usize i = 0; i < CAPACITY; i++) {
        buckets[i] = (struct LevelBucket *) Malloc(sizeof(struct LevelBucket));
        LevelBucket_Construct(buckets[i]);
    }
    struct Order order = { 0 };
    clock_t start = clock();
    for (usize i = 0; i < INSERTS; i++) {
        order.base.orderID = i;
        LevelBucket_GetOrAdd(buckets[rand() % CAPACITY], rand() % RANGE);
        // Level_Add(level, &order);
        if ((i % 8) == 0) {
            struct LevelBucket *bucket = buckets[rand() % CAPACITY];
            if (LevelBucket_Empty(bucket))
                continue;
            LevelBucket_Remove(bucket, LevelBucket_Peek(bucket));
        }
    }
    clock_t time = clock() - start;
    i32 milliseconds = time * 1000 / CLOCKS_PER_SEC;
    printf("Time %d\n", milliseconds);
    return 0;
}
