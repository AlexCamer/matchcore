#include <string.h>
#include "order.h"
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
OrderChunk_construct(struct OrderChunk *chunk) {
    chunk->next = NULL;
    chunk->prev = NULL;
    chunk->size = 0;
    for (u8 i = 0; i < ORDER_CHUNK_CAPACITY; i++)
        chunk->queue = i;
}

struct OrderChunk *
OrderChunk_new(void) {
    struct OrderChunk *chunk = (struct OrderChunk *) Pool_malloc(&OrderChunk_pool);
    OrderChunk_construct(chunk);
    return chunk;
}

void
OrderChunk_delete(struct OrderChunk *chunk) {
    Pool_free(&OrderChunk_pool, (void *) chunk);
}

void
OrderChunk_add(struct OrderChunk *chunk, struct Order *order) {
    u8 index = chunk->queue[chunk->size++];
    chunk->orders[index].orderID = order->orderID;
    chunk->orders[index].userID = order->userID;
    chunk->orders[index].volume = order->volume;
}

void
OrderChunk_remove(struct OrderChunk *chunk) {
    u8 front = chunk->queue[0];
    memcpy(chunk->queue, chunk->queue + 1, ORDER_CHUNK_CAPACITY - 1);
    chunk->queue[ORDER_CHUNK_CAPACITY - 1] = front;
}

void
OrderChunk_peek(struct OrderChunk *chunk) {

}
