#pragma once

#include "config.h"
#include "order.h"

struct Level;

struct OrderChunk {
    struct Level *level;
    struct OrderChunk *next;
    struct OrderChunk *prev;
    struct OrderBase orders[ORDER_CHUNK_CAPACITY];
    u8 queue[ORDER_CHUNK_CAPACITY];
    u8 size;
};

#define OrderChunk_Empty(chunk) ((chunk)->size == 0)
#define OrderChunk_Full(chunk) ((chunk)->size == ORDER_CHUNK_CAPACITY)

struct OrderChunk *OrderChunk_New(struct Level *level);
void OrderChunk_Delete(struct OrderChunk *chunk);
void OrderChunk_Add(struct OrderChunk *chunk, struct Order *order);
