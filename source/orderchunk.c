#include <string.h>
#include "order.h"
#include "orderchunk.h"

void OrderChunk_init(struct OrderChunk *oc) {
    oc->next = NULL;
    oc->prev = NULL;
    oc->size = 0;
    for (u8 i = 0; i < ORDER_CHUNK_CAPACITY; i++)
        oc->queue = i;
}

void OrderChunk_add(struct OrderChunk *oc, struct Order *o) {
    u8 index = oc->queue[oc->size++];
    oc->orders[index].orderID = o->orderID;
    oc->orders[index].userID = o->userID;
    oc->orders[index].volume = o->volume;
}

void OrderChunk_remove(struct OrderChunk *oc) {
    u8 front = oc->queue[0];
    memcpy(oc->queue, oc->queue + 1, ORDER_CHUNK_CAPACITY - 1);
    oc->queue[ORDER_CHUNK_CAPACITY - 1] = front;
}

void OrderChunk_peek(struct OrderChunk *oc);
