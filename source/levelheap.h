#pragma once

#include "config.h"
#include "levelbucket.h"
#include "macros.h"

#define LEVEL_HEAP_CAPACITY EXP2(LEVEL_HEAP_CAPACITY_LOG2)

struct Level;
struct Pool;

struct LevelHeap {
    struct Pool *pool;
    struct LevelBucket buckets[LEVEL_HEAP_CAPACITY];
    i32 prices[LEVEL_HEAP_CAPACITY];
    u16 index[LEVEL_HEAP_CAPACITY];
    usize size;
};

#define LevelHeap_empty(heap) ((heap)->size == 0)

void LevelHeap_construct(struct LevelHeap *heap);
void LevelHeap_destruct(struct LevelHeap *heap);
struct Level *LevelHeap_getOrAdd(struct LevelHeap *heap, i32 price);
struct Level *LevelHeap_peek(struct LevelHeap *heap);
void LevelHeap_remove(struct LevelHeap *heap, struct Level *level);
void LevelHeap_pop(struct LevelHeap *heap);
