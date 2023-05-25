#pragma once

#include "../include/types.h"

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

#define Level_empty(level) ((level)->volume == 0)

struct Level *Level_new(struct LevelHeap *heap, i32 price);
void Level_delete(struct Level *level);
void Level_add(struct Level *level, struct Order *order);
void Level_trade(struct Level *level, struct Order *order);
