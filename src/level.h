#pragma once

#include "types.h"

struct Levels;
struct Order;
struct OrderChunk;

enum LevelColor {
    RED = 0,
    BLACK = 1
};

struct Level {
    struct Levels *levels;
    struct Level *parent, *left, *right;
    struct OrderChunk *front, *back;
    enum LevelColor color:1;
    u64 volume:63;
    i32 price;
};

#define Level_empty(l) ((l)->front == NULL)

void Level_init(struct Level *l, struct Levels *ls, enum LevelColor lc, i32 price);
void Level_add(struct Level *l, struct Order *o);
void Level_remove(struct Level *l);
void Level_peek(struct Level *l);
