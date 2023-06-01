#pragma once

#include "types.h"

struct LevelHashHeap;
struct Order;
struct OrderChunk;

struct Level { /* layout optimized for cache locality */
    /* level tree members */
    i32 price, balance;
    struct Level *parent, *left, *right;

    /* order queue members */
    u64 volume;
    struct OrderChunk *front, *back;
    struct LevelHashHeap *heap;
};

#define Level_Peek(level) ((level)->front)

struct Level *Level_New(i32 price);
void Level_Delete(struct Level *level);
void Level_Add(struct Level *level, struct Order *order);
void Level_Pop(struct Level *level);
