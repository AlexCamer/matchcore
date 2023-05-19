#pragma once

#define ORDER_CHUNK_CAPACITY_LOG2 3
#define ORDER_CHUNK_CAPACITY EXP2(ORDER_CHUNK_CAPACITY_LOG2)

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

void OrderChunk_init(struct OrderChunk *oc);
void OrderChunk_add(struct OrderChunk *oc, struct Order *o);
void OrderChunk_remove(struct OrderChunk *oc);
void OrderChunk_peek(struct OrderChunk *oc);
