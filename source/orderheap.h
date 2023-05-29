#pragma once

#include "config.h"
#include "leveltree.h"
#include "macros.h"

#define ORDER_HEAP_CAPACITY EXP2(ORDER_HEAP_CAPACITY_LOG2)

struct Order;
struct OrderChunk;

struct OrderHeap {
    struct LevelBucket buckets[ORDER_HEAP_CAPACITY];
    i32 prices[ORDER_HEAP_CAPACITY];
    usize size;
};

#define OrderHeap_Empty(heap) ((heap)->size == 0)

void OrderHeap_Construct(struct OrderHeap *heap);
void OrderHeap_Destruct(struct OrderHeap *heap);
void OrderHeap_Add(struct OrderHeap *heap, struct Order *order);
void OrderHeap_Pop(struct OrderHeap *heap);
struct OrderChunk *OrderHeap_Peek(struct OrderHeap *heap);
