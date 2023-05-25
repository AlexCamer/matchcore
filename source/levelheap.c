#include <string.h>
#include "level.h"
#include "levelbucket.h"
#include "levelheap.h"
#include "pool.h"

#define LevelHeap_hash(price) ((usize) MOD_EXP2(price, LEVEL_HEAP_CAPACITY_LOG2))

static inline void
LevelHeap_swap(struct LevelHeap *heap, usize i, usize j) {
    i32 iPrice = heap->prices[i];
    i32 jPrice = heap->prices[j];
    heap->buckets[LevelHeap_hash(iPrice)].index = j;
    heap->buckets[LevelHeap_hash(jPrice)].index = i;
    heap->prices[i] = jPrice;
    heap->prices[j] = iPrice;
}

static inline void
LevelHeap_fixUp(struct LevelHeap *heap, usize i) {
    usize parent = DIV2(i);
    for (; heap->prices[i] > heap->prices[parent]; i = parent, parent = DIV2(parent))
        LevelHeap_swap(heap, i, parent);
}

static inline void
LevelHeap_fixDown(struct LevelHeap *heap, usize i) {
    for (;;) {
        usize best = i, left = MUL2(i), right = left + 1;
        if (left < heap->size && heap->prices[left] > heap->prices[best])
            best = left;
        if (right < heap->size && heap->prices[right] > heap->prices[best])
            best = right;
        if (best == i)
            return;
        LevelHeap_swap(heap, i, best);
        i = best;
    }
}

void
LevelHeap_construct(struct LevelHeap *heap, struct Pool *pool) {
    heap->pool = pool;
    heap->size = 0;
    memset(heap->buckets, 0, sizeof(heap->buckets));
}

void
LevelHeap_destruct(struct LevelHeap *heap) {
    for (usize i = 0; i < heap->size; i++)
        LevelBucket_destruct(heap->buckets + LevelHeap_hash(heap->prices[i]));
}

struct Level *
LevelHeap_getOrAdd(struct LevelHeap *heap, i32 price) {
    struct LevelBucket *bucket = heap->buckets + LevelHeap_hash(price);
    if (LevelBucket_empty(bucket)) {
        heap->prices[heap->size] = price;
        bucket->index = heap->size++;
        LevelHeap_fixUp(heap, bucket->index);
    } else if (price > heap->prices[bucket->index]) {
        heap->prices[bucket->index] = price;
        LevelHeap_fixUp(heap, bucket->index);
    }
    return LevelBucket_getOrAdd(bucket, heap, price);
}

struct Level *
LevelHeap_peek(struct LevelHeap *heap) {
    return LevelBucket_peek(heap->buckets + LevelHeap_hash(heap->prices[0]));
}

void
LevelHeap_remove(struct LevelHeap *heap, struct Level *level) {
    struct LevelBucket *bucket = heap->buckets + LevelHeap_hash(level->price);
    LevelBucket_remove(bucket, level);
    if (LevelBucket_empty(bucket)) {
        LevelHeap_swap(heap, bucket->index, --(heap->size));
        LevelHeap_fixUp(heap, bucket->index);
        LevelHeap_fixDown(heap, bucket->index);
    } else if (LevelBucket_peek(bucket)->price < heap->prices[bucket->index]) {
        heap->prices[bucket->index] = LevelBucket_peek(bucket)->price;
        LevelHeap_fixDown(heap, bucket->index);
    }
}
