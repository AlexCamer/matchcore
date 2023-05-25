#include "config.h"
#include "level.h"
#include "macros.h"
#include "order.h"
#include "orderchunk.h"
#include "pool.h"
#include "trade.h"

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
Level_construct(struct Level *level, struct LevelHeap *heap, i32 price) {
    level->heap = heap;
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
Level_new(struct LevelHeap *heap, i32 price) {
    struct Level *level = (struct Level *) Pool_malloc(&Level_pool);
    Level_construct(level, heap, price);
    return level;
}

void
Level_delete(struct Level *level) {
    Pool_free(&Level_pool, (void *) level);
}

void
Level_add(struct Level *level, struct Order *order) {
    if (level->back == NULL)
        level->front = level->back = OrderChunk_new(level);
    else if (OrderChunk_full(level->back))
        level->back->next = level->back = OrderChunk_new(level);
    OrderChunk_add(level->back, order);
}

void
Level_trade(struct Level *level, struct Order *order) {
    struct Trade trade;
    do {
        struct OrderNode *node = OrderChunk_peek(level->front);
        u32 volume = MIN(order->volume, node->volume);
        (order->side == BUY) ? Trade_construct(&trade, order->symbol, order->orderID, node->orderID, order->userID,
                                               node->userID, volume, level->price)
                             : Trade_construct(&trade, order->symbol, node->orderID, order->orderID, node->userID,
                                               order->userID, volume, level->price);
        Trade_handle(&trade);
        order->volume -= volume;
        node->volume -= volume;
        level->volume -= volume;
        if (node->volume == 0)
            OrderChunk_pop(level->front);
        if (OrderChunk_empty(level->front)) {
            struct OrderChunk *chunk = level->front;
            level->front = chunk->next;
            OrderChunk_delete(chunk);
        }
    } while (!Order_empty(order) && !Level_empty(level));
}
