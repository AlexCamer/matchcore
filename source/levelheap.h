#pragma once

#include "levelbucket.h"
#include "macros.h"

#define LEVELS_CAPACITY_LOG2 12
#define LEVELS_CAPACITY EXP2(LEVELS_CAPACITY_LOG2)

struct Level;
struct Pool;

struct LevelHeap {
    struct Pool *pool;
    struct LevelBucket buckets[LEVELS_CAPACITY];
    i32 prices[LEVELS_CAPACITY];
    u16 index[LEVELS_CAPACITY];
    usize size;
};

#define LevelHeap_empty(ls) ((ls)->size == 0)

void LevelHeap_init(struct LevelHeap *heap, struct Pool *pool);
struct Level *LevelHeap_getOrAdd(struct LevelHeap *heap, i32 price);
struct Level *LevelHeap_peek(struct LevelHeap *heap);
void LevelHeap_remove(struct LevelHeap *heap, struct Level *level);
void LevelHeap_pop(struct LevelHeap *heap);
