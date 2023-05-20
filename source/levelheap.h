#pragma once

#include "levelbucket.h"
#include "macros.h"

#define LEVEL_HEAP_CAPACITY_LOG2 13
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

#define LevelHeap_empty(ls) ((ls)->size == 0)

void LevelHeap_init(struct LevelHeap *heap, struct Pool *pool);
struct Level *LevelHeap_getOrAdd(struct LevelHeap *heap, i32 price);
struct Level *LevelHeap_peek(struct LevelHeap *heap);
void LevelHeap_remove(struct LevelHeap *heap, struct Level *level);
void LevelHeap_pop(struct LevelHeap *heap);
