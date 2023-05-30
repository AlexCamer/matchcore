#include <string.h>
#include "orderchunk.h"
#include "pool.h"
#include "trade.h"

static struct Pool orderChunkPool;

__attribute__((constructor))
static inline void
OrderChunk_ConstructPool(void) {
    Pool_Construct(&orderChunkPool, sizeof(struct OrderChunk), ORDER_CHUNK_POOL_ELEMENTS_PER_BLOCK);
}

__attribute__((destructor))
static inline void
OrderChunk_DestructPool(void) {
    Pool_Destruct(&orderChunkPool);
}

static inline void
OrderChunk_Construct(struct OrderChunk *chunk, struct Level *level) {
    chunk->level = level;
    chunk->next = NULL;
    chunk->prev = NULL;
    chunk->size = 0;
    for (u8 i = 0; i < ORDER_CHUNK_CAPACITY; i++)
        chunk->queue[i] = i;
}

struct OrderChunk *
OrderChunk_New(struct Level *level) {
    struct OrderChunk *chunk = (struct OrderChunk *) Pool_Malloc(&orderChunkPool);
    OrderChunk_Construct(chunk, level);
    return chunk;
}

void
OrderChunk_Delete(struct OrderChunk *chunk) {
    Pool_Free(&orderChunkPool, (void *) chunk);
}

void
OrderChunk_Add(struct OrderChunk *chunk, struct Order *order) {
    chunk->orders[chunk->queue[chunk->size++]] = order->base;
}
