#include "level.h"
#include "levelhashheap.h"

#define LEVEL_HASH_HEAP_CHILDREN EXP2(LEVEL_HASH_HEAP_CHILDREN_LOG2)

#define LevelHashHeap_Hash(price) ((usize) MOD_EXP2(price, LEVEL_HASH_HEAP_CAPACITY_LOG2))

static inline void
LevelHashHeap_Swap(struct LevelHashHeap *heap, usize i, usize j) {
    i32 iPrice = heap->prices[i];
    i32 jPrice = heap->prices[j];
    heap->prices[i] = jPrice;
    heap->prices[j] = iPrice;
    heap->buckets[LevelHashHeap_Hash(iPrice)].index = j;
    heap->buckets[LevelHashHeap_Hash(jPrice)].index = i;
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
        usize childrenEnd = MIN(childrenStart + LEVEL_HASH_HEAP_CHILDREN, heap->size);
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
LevelHashHeap_Construct(struct LevelHashHeap *heap) {
    heap->size = 0;
    for (usize i = 0; i < LEVEL_HASH_HEAP_CAPACITY; i++)
        LevelBucket_Construct(heap->buckets + i);
}

void
LevelHashHeap_Destruct(struct LevelHashHeap *heap) {
    for (usize i = 0; i < heap->size; i++)
        LevelBucket_Destruct(heap->buckets + LevelHashHeap_Hash(heap->prices[i]));
}

struct Level *
LevelHashHeap_GetOrAdd(struct LevelHashHeap *heap, i32 price) {
    struct LevelBucket *bucket = heap->buckets + LevelHashHeap_Hash(price);
    if (LevelBucket_Empty(bucket)) {
        bucket->index = heap->size++;
        heap->prices[bucket->index] = price;
        LevelHashHeap_FixUp(heap, bucket->index);
    } else if (price > LevelBucket_Peek(bucket)->price) {
        heap->prices[bucket->index] = price;
        LevelHashHeap_FixUp(heap, bucket->index);
    }
    return LevelBucket_GetOrAdd(bucket, price);
}

void
LevelHashHeap_Remove(struct LevelHashHeap *heap, struct Level *level) {
    struct LevelBucket *bucket = heap->buckets + LevelHashHeap_Hash(level->price);
    i32 price = LevelBucket_Peek(bucket)->price;
    LevelBucket_Remove(bucket, level);
    if (LevelBucket_Empty(bucket)) {
        usize i = bucket->index;
        LevelHashHeap_Swap(heap, i, --(heap->size));
        LevelHashHeap_FixUp(heap, i);
        LevelHashHeap_FixDown(heap, i);
    } else if (price > LevelBucket_Peek(bucket)->price) {
        heap->prices[bucket->index] = LevelBucket_Peek(bucket)->price;
        LevelHashHeap_FixDown(heap, bucket->index);
    }
}

struct Level *
LevelHashHeap_Peek(struct LevelHashHeap *heap) {
    return LevelBucket_Peek(heap->buckets + LevelHashHeap_Hash(heap->prices[0]));
}
