#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../source/level.h"
#include "../source/macros.h"
#include "../source/order.h"
#include "../source/orderchunk.h"

#define SEED 52387301
#define CAPACITY EXP2(8)
#define INSERTS EXP2(20)

i32 main(void) {
    printf("Size: %lu\n", sizeof(struct OrderChunk));
    struct Level *levels[CAPACITY];
    for (usize i = 0; i < CAPACITY; i++)
        levels[i] = Level_New(0);
    struct Order order = { 0 };
    clock_t start = clock();
    for (usize i = 0; i < INSERTS; i++) {
        order.base.orderID = i;
        Level_Add(levels[rand() % CAPACITY], &order);
    }
    clock_t time = clock() - start;
    i32 milliseconds = time * 1000 / CLOCKS_PER_SEC;
    printf("Time %d\n", milliseconds);
    return 0;
}
