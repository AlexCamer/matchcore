#pragma once

#include "types.h"

struct OrderBase;
struct OrderChunk;

struct Level {
    // struct LevelHeap *heap;
    struct Level *parent;
    struct Level *left;
    struct Level *right;
    struct OrderChunk *front;
    struct OrderChunk *back;
    i8 balance;
    u64 volume:56;
    i32 price;
};

#define Level_Peek(level) ((level)->front)

struct Level *Level_New(i32 price);
void Level_Delete(struct Level *level);
void Level_Add(struct Level *level, struct OrderBase *order);
void Level_Pop(struct Level *level);
