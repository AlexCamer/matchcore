#include "../include/engine/level.h"
#include "../source/orderheap.h"
#include "../include/utility/pool.h"
#include "../include/utility/alloc.h"

i32 main(i32 argc, char *argv[]) {
    struct Pool pool;
    Pool_init(&pool, sizeof(struct Level), EXP2(14));
    struct Level *ptrs[EXP2(12)];
    for (i32 j = 0; j < EXP2(12); j++) {
        for (i32 i = 0; i < EXP2(10); i++) {
            ptrs[i] = (struct Level *) Pool_Malloc(&pool);
            // ptrs[i] = (struct Level *) Malloc(sizeof(struct Level));
            ptrs[i]->price = 9;
        }
        for (i32 i = 0; i < EXP2(10); i++) {
            Pool_free(&pool, ptrs[i]);
            // xfree(ptrs[i]);
        }
    }
}
