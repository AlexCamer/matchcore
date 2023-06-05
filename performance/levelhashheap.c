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
#define RANGE 2048

i32 main(void) {
    printf("Size: %lu x %d = %lu\n", sizeof(struct LevelHashHeap),
                                     CAPACITY,
                                     sizeof(struct LevelHashHeap) * CAPACITY);
    srand(SEED);
    struct LevelHashHeap *heaps[CAPACITY];
    for (usize i = 0; i < CAPACITY; i++) {
        heaps[i] = Malloc(sizeof(struct LevelHashHeap));
        LevelHashHeap_Construct(heaps[i]);
    }
    struct Order order = { 0 };
    clock_t start = clock();
    for (usize i = 0; i < INSERTS; i++) {
        order.base.orderID = i;
        LevelHashHeap_GetOrAdd(heaps[rand() % CAPACITY], rand() % RANGE);
        // Level_Add(level, &order);
        if ((i % 8) == 0) {
            struct LevelHashHeap *heap = heaps[rand() % CAPACITY];
            if (LevelHashHeap_Empty(heap))
                continue;
            LevelHashHeap_Remove(heap, LevelHashHeap_Peek(heap));
        }
    }
    clock_t time = clock() - start;
    i32 milliseconds = time * 1000 / CLOCKS_PER_SEC;
    printf("Time %d\n", milliseconds);
    return 0;
}
