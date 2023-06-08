#pragma once

#include "config.h"
#include "leveltree.h"
#include "../utility/macros.h"

#define LEVEL_HASH_HEAP_CAPACITY EXP2(LEVEL_HASH_HEAP_CAPACITY_LOG2)
#if LEVEL_HASH_HEAP_CAPACITY_LOG2 <= 8
#define LEVEL_HASH_HEAP_POSITION_TYPE u8
#elif LEVEL_HASH_HEAP_CAPACITY_LOG2 <= 16
#define LEVEL_HASH_HEAP_POSITION_TYPE u16
#elif LEVEL_HASH_HEAP_CAPACITY_LOG2 <= 32
#define LEVEL_HASH_HEAP_POSITION_TYPE u32
#else
#define LEVEL_HASH_HEAP_POSITION_TYPE usize
#endif

struct LevelHashHeap {
    struct LevelTree buckets[LEVEL_HASH_HEAP_CAPACITY];
    i32 prices[LEVEL_HASH_HEAP_CAPACITY];
    LEVEL_HASH_HEAP_POSITION_TYPE positions[LEVEL_HASH_HEAP_CAPACITY];
    usize size;
};

#define LevelHashHeap_Empty(heap) ((heap)->size == 0)

void LevelHashHeap_Construct(struct LevelHashHeap *heap);
void LevelHashHeap_Destruct(struct LevelHashHeap *heap);
void LevelHashHeap_Remove(struct LevelHashHeap *heap, struct Level *level);
struct Level *LevelHashHeap_GetOrAdd(struct LevelHashHeap *heap, i32 price);
struct Level *LevelHashHeap_Peek(struct LevelHashHeap *heap);
