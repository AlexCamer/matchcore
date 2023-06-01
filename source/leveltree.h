#pragma once

#include "types.h"

struct Level;

struct LevelTree { /* layout optimized for cache locality */
    struct Level *best, *root;
};

#define LevelTree_Peek(tree) ((tree)->best)
#define LevelTree_Empty(tree) ((tree)->best == NULL)

void LevelTree_Construct(struct LevelTree *tree);
void LevelTree_Destruct(struct LevelTree *tree);
void LevelTree_Remove(struct LevelTree *tree, struct Level *level);
struct Level *LevelTree_GetOrAdd(struct LevelTree *tree, i32 price);
