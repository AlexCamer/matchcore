#pragma once

struct Level;
struct LevelHeap;

struct LevelTree {
    struct Level *root;
    struct Level *best;
};

#define LevelTree_peek(lt) ((lt)->best)
#define LevelTree_empty(lt) (!((lt)->root))

void LevelTree_init(struct LevelTree *lt);
struct Level *LevelTree_getOrAdd(struct LevelTree *lt, struct LevelHeap *lh, i32 price);
void LevelTree_remove(struct LevelTree *lt, struct Level *l);
void LevelTree_pop(struct LevelTree *lt);
