#pragma once

#include <unordered_map>
#include <map>
#include <vector>

#include "config.h"
#include "levelbucket.h"
#include "macros.h"

#define LEVEL_HASH_HEAP_CAPACITY EXP2(LEVEL_HASH_HEAP_CAPACITY_LOG2)

struct IndexedLevel {
    usize index;
    struct Level *level;
};

struct LevelHashHeap {
    std::unordered_map<i32, IndexedLevel> map;
    std::vector<i32> prices;
};

#define LevelHashHeap_Empty(heap) ((heap)->prices.size() == 0)

void LevelHashHeap_Construct(LevelHashHeap *heap);
void LevelHashHeap_Destruct(LevelHashHeap *heap);
void LevelHashHeap_Remove(LevelHashHeap *heap, struct Level *level);
struct Level *LevelHashHeap_GetOrAdd(LevelHashHeap *heap, i32 price);
struct Level *LevelHashHeap_Peek(LevelHashHeap *heap);
