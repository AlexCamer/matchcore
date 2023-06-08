#include "../../include/engine/level.h"
#include "../../include/engine/levelhashheap.h"

#define LEVEL_HASH_HEAP_CHILDREN EXP2(LEVEL_HASH_HEAP_CHILDREN_LOG2)

#define LevelHashHeap_Hash(price) ((usize) MOD_EXP2(price, LEVEL_HASH_HEAP_CAPACITY_LOG2))

static inline void
LevelHashHeap_Swap(struct LevelHashHeap *heap, usize i, usize j) {
    i32 iPrice = heap->prices[i];
    i32 jPrice = heap->prices[j];
    heap->prices[i] = jPrice;
    heap->prices[j] = iPrice;
    heap->positions[LevelHashHeap_Hash(iPrice)] = j;
    heap->positions[LevelHashHeap_Hash(jPrice)] = i;
}

static inline void
LevelHashHeap_FixUp(struct LevelHashHeap *heap, usize i) {
    for (; i > 0;) {
        usize parent = DIV_EXP2(i - 1, LEVEL_HASH_HEAP_CHILDREN_LOG2);
        if (heap->prices[i] <= heap->prices[parent])
            return;
        LevelHashHeap_Swap(heap, i, parent);
        i = parent;
    }
}

static inline void
LevelHashHeap_FixDown(struct LevelHashHeap *heap, usize i) {
    for (usize best = i;; i = best, best = i) {
        usize childrenStart = MUL_EXP2(i, LEVEL_HASH_HEAP_CHILDREN_LOG2) + 1;
        usize childrenEnd = childrenStart + EXP2(LEVEL_HASH_HEAP_CHILDREN_LOG2);
        /*if (childrenStart < heap->size && heap->prices[childrenStart] > heap->prices[best])
            best = childrenStart;
        if (childrenStart + 1 < heap->size && heap->prices[childrenStart + 1] > heap->prices[best])
            best = childrenStart + 1;*/
        for (usize child = childrenStart; child < childrenEnd && child < heap->size; child++) {
            if (heap->prices[child] > heap->prices[best])
                best = child;
        }
        if (best == i)
            return;
        LevelHashHeap_Swap(heap, i, best);
    }
}

/*static inline void
LevelHashHeap_FixDown(struct LevelHashHeap *heap, usize i) {
    for (;;) {
        usize best = i, left = MUL2(i) + 1, right = left + 1;
        if (left < heap->size && heap->prices[left] > heap->prices[best])
            best = left;
        if (right < heap->size && heap->prices[right] > heap->prices[best])
            best = right;
        printf("%lu\n", best);
        if (best == i)
            return;
        LevelHashHeap_Swap(heap, i, best);
        i = best;
    }
}*/

void
LevelHashHeap_Construct(struct LevelHashHeap *heap) {
    heap->size = 0;
    for (usize i = 0; i < LEVEL_HASH_HEAP_CAPACITY; i++)
        LevelTree_Construct(heap->buckets + i);
}

void
LevelHashHeap_Destruct(struct LevelHashHeap *heap) {
    for (usize i = 0; i < heap->size; i++)
        LevelTree_Destruct(heap->buckets + LevelHashHeap_Hash(heap->prices[i]));
}

struct Level *
LevelHashHeap_GetOrAdd(struct LevelHashHeap *heap, i32 price) {
    usize hash = LevelHashHeap_Hash(price);
    struct LevelTree *bucket = heap->buckets + hash;
    if (LevelTree_Empty(bucket)) {
        heap->prices[heap->size] = price;
        heap->positions[hash] = heap->size;
        LevelHashHeap_FixUp(heap, heap->size++);
    } else if (price > LevelTree_Peek(bucket)->price) {
        usize i = heap->positions[hash];
        heap->prices[i] = price;
        LevelHashHeap_FixUp(heap, i);
    }
    return LevelTree_GetOrAdd(bucket, price);
}

void
LevelHashHeap_Remove(struct LevelHashHeap *heap, struct Level *level) {
    usize hash = LevelHashHeap_Hash(level->price), i = heap->positions[hash];
    struct LevelTree *bucket = heap->buckets + hash;
    i32 price = LevelTree_Peek(bucket)->price;
    LevelTree_Remove(bucket, level);
    if (LevelTree_Empty(bucket)) {
        LevelHashHeap_Swap(heap, i, --(heap->size));
        LevelHashHeap_FixUp(heap, i);
        LevelHashHeap_FixDown(heap, i);
    } else if (price > LevelTree_Peek(bucket)->price) {
        heap->prices[i] = LevelTree_Peek(bucket)->price;
        LevelHashHeap_FixDown(heap, i);
    }
}

struct Level *
LevelHashHeap_Peek(struct LevelHashHeap *heap) {
    return LevelTree_Peek(heap->buckets + LevelHashHeap_Hash(heap->prices[0]));
}
