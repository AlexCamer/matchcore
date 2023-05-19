#pragma once

struct Level;
struct LevelHeap;

struct LevelBucket {
    struct Level *root;
    struct Level *best;
};

#define LevelBucket_peek(bucket) ((bucket)->best)
#define LevelBucket_empty(bucket) ((bucket)->root == NULL)

void LevelBucket_init(struct LevelBucket *bucket);
struct Level *LevelBucket_getOrAdd(struct LevelBucket *bucket, struct LevelHeap *heap, i32 price);
void LevelBucket_remove(struct LevelBucket *bucket, struct Level *level);
