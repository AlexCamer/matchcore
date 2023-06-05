#pragma once

#include "config.h"
#include "levelbucket.h"
#include "macros.h"

#define LEVEL_HASH_HEAP_CAPACITY EXP2(LEVEL_HASH_HEAP_CAPACITY_LOG2)

struct LevelHashHeap {
    struct LevelBucket buckets[LEVEL_HASH_HEAP_CAPACITY];
    i32 prices[LEVEL_HASH_HEAP_CAPACITY];
    usize size;
};

#define LevelHashHeap_Empty(heap) ((heap)->size == 0)

void LevelHashHeap_Construct(struct LevelHashHeap *heap);
void LevelHashHeap_Destruct(struct LevelHashHeap *heap);
void LevelHashHeap_Remove(struct LevelHashHeap *heap, struct Level *level);
struct Level *LevelHashHeap_GetOrAdd(struct LevelHashHeap *heap, i32 price);
struct Level *LevelHashHeap_Peek(struct LevelHashHeap *heap);
