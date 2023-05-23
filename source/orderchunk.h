#pragma once

#include "config.h"

struct Order;

struct OrderChunk {
    struct OrderChunk *next, *prev;
    struct {
        u64 orderID;
        u32 userID;
        u32 volume;
    } orders[ORDER_CHUNK_CAPACITY];
    u8 size, queue[ORDER_CHUNK_CAPACITY];
};

#define OrderChunk_empty(oc) ((oc)->size == 0)
#define OrderChunk_full(oc) ((oc)->size == ORDER_CHUNK_CAPACITY)

struct OrderChunk *OrderChunk_new(void);
void OrderChunk_delete(struct OrderChunk *chunk);
void OrderChunk_add(struct OrderChunk *chunk, struct Order *order);
void OrderChunk_pop(struct OrderChunk *chunk);
void OrderChunk_peek(struct OrderChunk *chunk);
