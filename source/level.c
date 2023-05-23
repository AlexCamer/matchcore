#include "config.h"
#include "level.h"
#include "pool.h"

static struct Pool Level_pool;

__attribute__((constructor))
static inline void
Level_constructPool(void) {
    Pool_construct(&Level_pool, sizeof(struct Level), LEVEL_POOL_ELEMENTS_PER_BLOCK);
}

__attribute__((destructor))
static inline void
Level_destructPool(void) {
    Pool_destruct(&Level_pool);
}

static inline void
Level_construct(struct Level *level, i32 price) {
    level->heap = NULL;
    level->parent = NULL;
    level->left = NULL;
    level->right = NULL;
    level->front = NULL;
    level->back = NULL;
    level->balance = 0;
    level->volume = 0;
    level->price = price;
}

struct Level *
Level_new(i32 price) {
    struct Level *level = (struct Level *) Pool_malloc(&Level_pool);
    Level_construct(level, price);
    return level;
}

void
Level_delete(struct Level *level) {
    Pool_free(&Level_pool, (void *) level);
}

void
Level_add(struct Level *level, struct Order *order) {

}

void
Level_pop(struct Level *level) {

}

void
Level_peek(struct Level *level) {

}
