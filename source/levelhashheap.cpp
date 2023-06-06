#include <iostream>
#include "level.h"
#include "levelhashheap.h"

#define LEVEL_HASH_HEAP_CHILDREN EXP2(LEVEL_HASH_HEAP_CHILDREN_LOG2)

static inline void
LevelHashHeap_Swap(LevelHashHeap *heap, usize i, usize j) {
    i32 iPrice = heap->prices[i];
    i32 jPrice = heap->prices[j];
    heap->prices[i] = jPrice;
    heap->prices[j] = iPrice;
    heap->map[iPrice].index = j;
    heap->map[jPrice].index = i;
}

static inline void
LevelHashHeap_FixUp(LevelHashHeap *heap, usize i) {
    for (; i > 0;) {
        usize parent = DIV_EXP2(i - 1, LEVEL_HASH_HEAP_CHILDREN_LOG2);
        if (heap->prices[i] <= heap->prices[parent])
            return;
        LevelHashHeap_Swap(heap, i, parent);
        i = parent;
    }
}

static inline void
LevelHashHeap_FixDown(LevelHashHeap *heap, usize i) {
    for (usize best = i;; i = best, best = i) {
        usize childrenStart = MUL_EXP2(i, LEVEL_HASH_HEAP_CHILDREN_LOG2) + 1;
        usize childrenEnd = MIN(childrenStart + LEVEL_HASH_HEAP_CHILDREN, heap->prices.size());
        for (usize child = childrenStart; child < childrenEnd; child++) {
            if (heap->prices[child] > heap->prices[best])
                best = child;
        }
        if (best == i)
            return;
        LevelHashHeap_Swap(heap, i, best);
    }
}

void
LevelHashHeap_Construct(LevelHashHeap *heap) {
}

void
LevelHashHeap_Destruct(LevelHashHeap *heap) {
}

struct Level *
LevelHashHeap_GetOrAdd(LevelHashHeap *heap, i32 price) {
    if (heap->map.count(price))
        return heap->map[price].level;
    heap->map[price].index = heap->prices.size();
    heap->map[price].level = Level_New(price);
    heap->prices.push_back(price);
    LevelHashHeap_FixUp(heap, heap->prices.size() - 1);
    return heap->map[price].level;
}

void
LevelHashHeap_Remove(LevelHashHeap *heap, struct Level *level) {
    if (level->price == heap->prices.back()) {
        heap->map.erase(level->price);
        heap->prices.pop_back();
        return;
    }
    size_t index = heap->map[level->price].index;
    LevelHashHeap_Swap(heap, index, heap->prices.size() - 1);
    heap->map.erase(level->price);
    heap->prices.pop_back();
    if (heap->prices[index] > level->price)
        LevelHashHeap_FixUp(heap, index);
    else
        LevelHashHeap_FixDown(heap, index);
    Level_Delete(level);
}

struct Level *
LevelHashHeap_Peek(LevelHashHeap *heap) {
    return heap->map[heap->prices[0]].level;
}
