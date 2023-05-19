#pragma once

#include "types.h"

struct LevelHeap;
struct Order;
struct OrderChunk;

struct Level {
    struct LevelHeap *heap;
    struct Level *parent, *left, *right;
    struct OrderChunk *front, *back;
    i8 balance;
    u64 volume:56;
    i32 price;
};

#define Level_empty(level) ((level)->front == NULL)

void Level_init(struct Level *level, struct LevelHeap *heap, i32 price);
void Level_add(struct Level *level, struct Order *o);
void Level_remove(struct Level *level);
void Level_peek(struct Level *level);
