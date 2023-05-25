#include <string.h>
#include "../include/order.h"
#include "orderchunk.h"
#include "pool.h"

static struct Pool OrderChunk_pool;

__attribute__((constructor))
static inline void
OrderChunk_constructPool(void) {
    Pool_construct(&OrderChunk_pool, sizeof(struct OrderChunk_pool), ORDER_CHUNK_POOL_ELEMENTS_PER_BLOCK);
}

__attribute__((destructor))
static inline void
OrderChunk_destructPool(void) {
    Pool_destruct(&OrderChunk_pool);
}

static inline void
OrderChunk_construct(struct OrderChunk *chunk, struct Level *level) {
    chunk->level = level;
    chunk->next = NULL;
    chunk->prev = NULL;
    chunk->size = 0;
    for (u8 i = 0; i < ORDER_CHUNK_CAPACITY; i++)
        chunk->queue = i;
}

struct OrderChunk *
OrderChunk_new(struct Level *level) {
    struct OrderChunk *chunk = (struct OrderChunk *) Pool_malloc(&OrderChunk_pool);
    OrderChunk_construct(chunk, level);
    return chunk;
}

void
OrderChunk_delete(struct OrderChunk *chunk) {
    Pool_free(&OrderChunk_pool, (void *) chunk);
}

void
OrderChunk_add(struct OrderChunk *chunk, struct Order *order) {
    struct OrderNode *node = chunk->orders[chunk->queue[chunk->size++]];
    node->orderID = order->orderID;
    node->userID = order->userID;
    node->volume = order->volume;
}

void
OrderChunk_pop(struct OrderChunk *chunk) {
    u8 front = chunk->queue[0];
    memcpy(chunk->queue, chunk->queue + 1, ORDER_CHUNK_CAPACITY - 1);
    chunk->queue[ORDER_CHUNK_CAPACITY - 1] = front;
}

struct OrderNode *
OrderChunk_peek(struct OrderChunk *chunk) {
    return chunk->orders[chunk->queue[0]];
}
