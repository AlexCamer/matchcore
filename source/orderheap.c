#include <string.h>
#include "level.h"
#include "levelbucket.h"
#include "levelheap.h"
#include "pool.h"

#define LevelHeap_hash(price) ((usize) MOD_EXP2(price, LEVEL_HEAP_CAPACITY_LOG2))

static inline void
LevelHeap_Swap(struct LevelHeap *heap, usize i, usize j) {
    i32 iPrice = heap->prices[i];
    i32 jPrice = heap->prices[j];
    heap->buckets[LevelHeap_hash(iPrice)].index = j;
    heap->buckets[LevelHeap_hash(jPrice)].index = i;
    heap->prices[i] = jPrice;
    heap->prices[j] = iPrice;
}

static inline void
LevelHeap_FixUp(struct LevelHeap *heap, usize i) {
    usize parent = DIV2(i);
    for (; heap->prices[i] > heap->prices[parent]; i = parent, parent = DIV2(parent))
        LevelHeap_Swap(heap, i, parent);
}

static inline void
LevelHeap_FixDown(struct LevelHeap *heap, usize i) {
    for (;;) {
        usize best = i, left = MUL2(i), right = left + 1;
        if (left < heap->size && heap->prices[left] > heap->prices[best])
            best = left;
        if (right < heap->size && heap->prices[right] > heap->prices[best])
            best = right;
        if (best == i)
            return;
        LevelHeap_Swap(heap, i, best);
        i = best;
    }
}

void
LevelHeap_Construct(struct LevelHeap *heap, struct Pool *pool) {
    heap->pool = pool;
    heap->size = 0;
    memset(heap->buckets, 0, sizeof(heap->buckets));
}

void
LevelHeap_Destruct(struct LevelHeap *heap) {
    for (usize i = 0; i < heap->size; i++)
        LevelBucket_Destruct(heap->buckets + LevelHeap_hash(heap->prices[i]));
}

struct Level *
LevelHeap_GetOrAdd(struct LevelHeap *heap, i32 price) {
    struct LevelBucket *bucket = heap->buckets + LevelHeap_hash(price);
    if (LevelBucket_Empty(bucket)) {
        heap->prices[heap->size] = price;
        bucket->index = heap->size++;
        LevelHeap_FixUp(heap, bucket->index);
    } else if (price > heap->prices[bucket->index]) {
        heap->prices[bucket->index] = price;
        LevelHeap_FixUp(heap, bucket->index);
    }
    return LevelBucket_GetOrAdd(bucket, heap, price);
}

struct Level *
LevelHeap_Peek(struct LevelHeap *heap) {
    return LevelBucket_Peek(heap->buckets + LevelHeap_hash(heap->prices[0]));
}

void
LevelHeap_Remove(struct LevelHeap *heap, struct Level *level) {
    struct LevelBucket *bucket = heap->buckets + LevelHeap_hash(level->price);
    LevelBucket_Remove(bucket, level);
    if (LevelBucket_Empty(bucket)) {
        LevelHeap_Swap(heap, bucket->index, --(heap->size));
        LevelHeap_FixUp(heap, bucket->index);
        LevelHeap_FixDown(heap, bucket->index);
    } else if (LevelBucket_Peek(bucket)->price < heap->prices[bucket->index]) {
        heap->prices[bucket->index] = LevelBucket_Peek(bucket)->price;
        LevelHeap_FixDown(heap, bucket->index);
    }
}
