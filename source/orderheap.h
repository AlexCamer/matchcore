#pragma once

#include "config.h"
#include "levelbucket.h"
#include "macros.h"

#define LEVEL_HEAP_CAPACITY EXP2(LEVEL_HEAP_CAPACITY_LOG2)

struct Order;

struct LevelHeap {
    struct LevelBucket buckets[LEVEL_HEAP_CAPACITY];
    i32 prices[LEVEL_HEAP_CAPACITY];
    usize size;
};

#define LevelHeap_Empty(heap) ((heap)->size == 0)

void LevelHeap_Construct(struct LevelHeap *heap);
void LevelHeap_Destruct(struct LevelHeap *heap);
struct Level *LevelHeap_GetOrAdd(struct LevelHeap *heap, i32 price);
struct Level *LevelHeap_Peek(struct LevelHeap *heap);
void LevelHeap_Remove(struct LevelHeap *heap, struct Level *level);
