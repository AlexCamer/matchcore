#pragma once

#include "types.h"

struct Level;

struct LevelTree {
    struct Level *root;
    struct Level *best;
};

#define LevelTree_Peek(tree) ((tree)->best)
#define LevelTree_Empty(tree) ((tree)->root == NULL)

void LevelTree_Construct(struct LevelTree *tree);
void LevelTree_Destruct(struct LevelTree *tree);
void LevelTree_Remove(struct LevelTree *tree, struct Level *level);
struct Level *LevelTree_GetOrAdd(struct LevelTree *tree, i32 price);
struct Level *LevelTree_AddBest(struct LevelTree *tree, i32 price);
