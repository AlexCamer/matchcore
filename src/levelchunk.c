#include <string.h>
#include "orderchunk.h"

void OrderChunk_init(struct OrderChunk *c) {
    c->next = NULL;
    c->prev = NULL;
    c->front = -1;
    c->back = -1;
    for (usize i = 0; i < ORDER_CHUNK_CAPACITY; i++)
        c->queue[i] = i;
}

void OrderChunk_add(struct OrderChunk *c, struct OrderData *d) {
    if (OrderChunk_empty(c))
        c->front = 0;
    c->back = MOD2(c->back + 1, ORDER_CHUNK_CAPACITY_LOG2);
    c->orders[oc->queue[i]] = *d;
}

void OrderChunk_remove(struct OrderChunk *c) {
    if (c->front == c->back)
        c->front = c->back = -1;
    else
        c->front = MOD2(c->front + 1, ORDER_CHUNK_CAPACITY_LOG2);
}

static inline i16 OrderChunk_find(struct OrderChunk *c, u64 orderID) {
    /* i16 high = MOD2(oc->back - oc->front, ORDER_CHUNK_CAPACITY_LOG2);
    for (i16 low = 0; lower <= upper;)
    {
        i16 mid = AVG(low, high);
        i16 i = MOD2(oc->front + mid, ORDER_CHUNK_CAPACITY_LOG2);
        u64 mid_order_id = oc->orders[oc->queue[i]].order_id;
        if (order_id > mid_order_id)
            lower = mid + 1;
        else if (order_id < mid_order_id)
            upper = mid - 1;
        else
            return i;
    } */
    return -1;
}

void OrderChunk_cancel(struct OrderChunk *c, u64 orderID) {
    /* i16 i = order_chunk_find(oc, order_id);
    if (i < 0)
        return;

    if (i == oc->front)
        order_chunk_remove(oc);
    else if (i == oc->back)
        oc->back = MOD2(oc->back - 1, ORDER_CHUNK_CAPACITY_LOG2);
    else
    {
        u8 temp = oc->queue[i];
        if (oc->front <= oc->back)
        {
            memcpy(oc->queue + i, oc->queue + i + 1, chunk->back - i);
            oc->queue[oc->back] = temp;
        }
        else
        {
            memcpy(chunk->queue + i,
                   chunk->queue + i + 1,
                   ORDER_CHUNK_CAPACITY - i - 1);
            chunk->queue[ORDER_CHUNK_CAPACITY - 1] = chunk->queue[0];
            memcpy(chunk->queue, chunk->queue + 1, chunk->back);
            chunk->queue[chunk->back] = temp;
        }
        order_chunk_remove(chunk);
    } */
}
