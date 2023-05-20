#include <string.h>
#include "level.h"
#include "levelbucket.h"
#include "levelheap.h"
#include "pool.h"

#define hash(price) ((usize) MOD_EXP2(price, LEVEL_HEAP_CAPACITY_LOG2))

static inline void swap(struct LevelHeap *heap, usize i, usize j) {
    i32 iPrice = heap->prices[i];
    i32 jPrice = heap->prices[j];
    heap->buckets[hash(iPrice)].index = j;
    heap->buckets[hash(jPrice)].index = i;
    heap->prices[i] = jPrice;
    heap->prices[j] = iPrice;
} /* swap */

static inline void fixUp(struct LevelHeap *heap, usize i) {
    usize parent = DIV2(i);
    for (; heap->prices[i] > heap->prices[parent]; i = parent, parent = DIV2(parent))
        swap(heap, i, parent);
} /* fixUp */

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
    } /* for */
} /* fixDown */

void LevelHeap_init(struct LevelHeap *heap, struct Pool *pool) {
    heap->pool = pool;
    heap->size = 0;
    memset(heap->buckets, 0, sizeof(heap->buckets));
} /* LevelHeap_init */

struct Level *LevelHeap_getOrAdd(struct LevelHeap *heap, i32 price) {
    struct LevelBucket *bucket = heap->buckets + hash(price);
    if (LevelBucket_empty(bucket)) {
        heap->prices[heap->size] = price;
        bucket->index = heap->size++;
        fixUp(heap, bucket->index);
    } else if (price > heap->prices[bucket->index]) {
        heap->prices[bucket->index] = price;
        fixUp(heap, bucket->index);
    } /* if */
    return LevelBucket_getOrAdd(bucket, heap, price);
} /* LevelHeap_getOrAdd */

struct Level *LevelHeap_peek(struct LevelHeap *heap) {
    return LevelBucket_peek(heap->buckets + hash(heap->prices[0]));
} /* LevelHeap_peek */

void LevelHeap_remove(struct LevelHeap *heap, struct Level *level) {
    struct LevelBucket *bucket = heap->buckets + hash(level->price);
    LevelBucket_remove(bucket, level);
    if (LevelBucket_empty(bucket)) {
        swap(heap, bucket->index, --(heap->size));
        fixUp(heap, bucket->index);
        fixDown(heap, bucket->index);
    } else if (LevelBucket_peek(bucket)->price < heap->prices[bucket->index]) {
        heap->prices[bucket->index] = LevelBucket_peek(bucket)->price;
        fixDown(heap, bucket->index);
    } /* if */
} /* LevelHeap_remove */

void LevelHeap_pop(struct LevelHeap *heap) {
    LevelHeap_remove(heap, LevelBucket_peek(heap->buckets + hash(heap->prices[0])));
} /* LevelHeap_pop */
