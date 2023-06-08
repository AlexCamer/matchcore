#pragma once

#include "../utility/types.h"

struct Level;

struct LevelBucket { /* layout optimized for cache locality */
    struct Level *best;
    struct Level *root;
    usize index;
};

#define LevelBucket_Peek(bucket) ((bucket)->best)
#define LevelBucket_Empty(bucket) ((bucket)->best == NULL)

void LevelBucket_Construct(struct LevelBucket *bucket);
void LevelBucket_Destruct(struct LevelBucket *bucket);
void LevelBucket_Remove(struct LevelBucket *bucket, struct Level *level);
struct Level *LevelBucket_GetOrAdd(struct LevelBucket *bucket, i32 price);
