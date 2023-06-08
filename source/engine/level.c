#include "../../include/engine/config.h"
#include "../../include/engine/level.h"
#include "../../include/utility/macros.h"
#include "../../include/engine/order.h"
#include "../../include/engine/orderchunk.h"
#include "../../include/utility/pool.h"
#include "../../include/engine/trade.h"

static struct Pool levelPool;

__attribute__((constructor))
static inline void
Level_ConstructPool(void) {
    Pool_Construct(&levelPool, sizeof(struct Level), LEVEL_POOL_ELEMENTS_PER_BLOCK);
}

__attribute__((destructor))
static inline void
Level_DestructPool(void) {
    Pool_Destruct(&levelPool);
}

static inline void
Level_Construct(struct Level *level, i32 price) {
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
Level_New(i32 price) {
    struct Level *level = (struct Level *) Pool_Malloc(&levelPool);
    Level_Construct(level, price);
    return level;
}

void
Level_Delete(struct Level *level) {
    Pool_Free(&levelPool, (void *) level);
}

void
Level_Add(struct Level *level, struct Order *order) {
    level->volume += order->base.volume;
    if (level->back == NULL)
        level->front = level->back = OrderChunk_New(level);
    else if (OrderChunk_Full(level->back))
        level->back = level->back->next = OrderChunk_New(level);
    OrderChunk_Add(level->back, order);
}

void
Level_Pop(struct Level *level) {
    /* struct OrderChunk *chunk = level->front;
    level->front = chunk->next;
    OrderChunk_Delete(chunk); */
}
