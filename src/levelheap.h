#pragma once

#include "leveltree.h"
#include "macros.h"

#define LEVELS_CAPACITY_LOG2 12
#define LEVELS_CAPACITY EXP2(LEVELS_CAPACITY_LOG2)

struct Level;
struct Pool;

struct LevelHeap {
    struct Pool *pool;
    struct LevelTree trees[LEVELS_CAPACITY];
    i32 prices[LEVELS_CAPACITY];
    u16 index[LEVELS_CAPACITY];
    usize size;
};

#define LevelHeap_empty(ls) ((ls)->size == 0)

void LevelHeap_init(struct LevelHeap *lh);
struct Level *LevelHeap_getOrAdd(struct LevelHeap *lh, i32 price);
struct Level *LevelHeap_peek(struct LevelHeap *lh);
void LevelHeap_remove(struct LevelHeap *lh, struct Level *l);
void LevelHeap_pop(struct LevelHeap *lh);
