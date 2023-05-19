#include <string.h>
#include "level.h"
#include "levelbucket.h"
#include "levelheap.h"
#include "pool.h"

#define LevelHeap_hash(price) ((usize) MOD_EXP2(price, LEVELS_CAPACITY_LOG2))

static inline void swap(struct LevelHeap *heap, usize i, usize j) {
    i32 iPrice = heap->prices[i];
    i32 jPrice = heap->prices[j];
    heap->index[LevelHeap_hash(iPrice)] = j;
    heap->index[LevelHeap_hash(jPrice)] = i;
    heap->prices[i] = jPrice;
    heap->prices[j] = iPrice;
}

static inline void fixUp(struct LevelHeap *heap, usize i) {
    usize parent = DIV2(i);
    for (; heap->prices[i] > heap->prices[parent]; i = parent, parent = DIV2(parent))
        swap(heap, i, parent);
}

static inline void fixDown(struct LevelHeap *heap, usize i) {
    for (;;) {
        usize best = i, left = MUL2(i), right = left + 1;
        if (left < heap->size && heap->prices[left] > heap->prices[best])
            best = left;
        if (right < heap->size && heap->prices[right] > heap->prices[best])
            best = right;
        if (best == i)
            return;
        swap(heap, i, best);
        i = best;
    }
}

void LevelHeap_init(struct LevelHeap *heap, struct Pool *pool) {
    heap->pool = pool;
    heap->size = 0;
    memset(heap->buckets, 0, sizeof(heap->buckets));
}

struct Level *LevelHeap_getOrAdd(struct LevelHeap *heap, i32 price) {
    usize hash = LevelHeap_hash(price), index = (usize) heap->index[hash];
    struct LevelBucket *bucket = heap->buckets + hash;
    if (LevelBucket_empty(bucket)) {
        heap->prices[heap->size] = price;
        heap->index[hash] = heap->size;
        fixUp(heap, heap->size++);
    } else {
        if (price == heap->prices[index])
            return LevelBucket_peek(bucket);
        if (price > heap->prices[index]) {
            heap->prices[index] = price;
            fixUp(heap, index);
        }
    }
    return LevelBucket_getOrAdd(bucket, heap, price);
}

struct Level *LevelHeap_peek(struct LevelHeap *heap) {
    return LevelBucket_peek(heap->buckets + LevelHeap_hash(heap->prices[0]));
}

void LevelHeap_remove(struct LevelHeap *heap, struct Level *level) {
    usize hash = LevelHeap_hash(level->price), index = (usize) heap->index[hash];
    struct LevelBucket *bucket = heap->buckets + hash;
    LevelBucket_remove(bucket, level);
    if (LevelBucket_empty(bucket)) {
        swap(heap, index, --(heap->size));
        fixUp(heap, index);
        fixDown(heap, index);
    } else if (LevelBucket_peek(bucket)->price < heap->prices[index]) {
        heap->prices[index] = LevelBucket_peek(bucket)->price;
        fixDown(heap, index);
    }
}

void LevelHeap_pop(struct LevelHeap *heap) {
    LevelHeap_remove(heap, LevelBucket_peek(heap->buckets + LevelHeap_hash(heap->prices[0])));
}
