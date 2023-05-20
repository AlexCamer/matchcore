#include "../source/level.h"
#include "../source/levelheap.h"
#include "../source/pool.h"

i32 main(i32 argc, char *argv[]) {
    struct Pool pool;
    Pool_init(&pool, sizeof(struct Level), EXP2(15));
    struct LevelHeap heap;
    LevelHeap_init(&heap, &pool);
    for (i32 i = 0; i < EXP2(18); i++) {
        LevelHeap_getOrAdd(&heap, i)->volume = 1;
    }
}
