#include <string.h>
#include "level.h"
#include "leveltree.h"
#include "orderchunk.h"
#include "orderheap.h"

#define OrderHeap_hash(price) ((usize) MOD_EXP2(price, LEVEL_HEAP_CAPACITY_LOG2))

static inline void
OrderHeap_Swap(struct OrderHeap *heap, usize i, usize j) {
    i32 iPrice = heap->prices[i];
    i32 jPrice = heap->prices[j];
    heap->buckets[OrderHeap_hash(iPrice)].index = j;
    heap->buckets[OrderHeap_hash(jPrice)].index = i;
    heap->prices[i] = jPrice;
    heap->prices[j] = iPrice;
}

static inline void
OrderHeap_FixUp(struct OrderHeap *heap, usize i) {
    usize parent = DIV2(i);
    for (; heap->prices[i] > heap->prices[parent]; i = parent, parent = DIV2(parent))
        OrderHeap_Swap(heap, i, parent);
}

static inline void
OrderHeap_FixDown(struct OrderHeap *heap, usize i) {
    for (;;) {
        usize best = i, left = MUL2(i), right = left + 1;
        if (left < heap->size && heap->prices[left] > heap->prices[best])
            best = left;
        if (right < heap->size && heap->prices[right] > heap->prices[best])
            best = right;
        if (best == i)
            return;
        OrderHeap_Swap(heap, i, best);
        i = best;
    }
}

void
OrderHeap_Construct(struct OrderHeap *heap) {
    heap->size = 0;
    memset(heap->buckets, 0, sizeof(heap->buckets));
}

void
OrderHeap_Destruct(struct OrderHeap *heap) {
    for (usize i = 0; i < heap->size; i++)
        LevelBucket_Destruct(heap->buckets + LevelHeap_hash(heap->prices[i]));
}

void
OrderHeap_Add(struct OrderHeap *heap, struct Order *order) {
    i32 price = order->price;
    struct LevelBucket *bucket = heap->buckets + LevelHeap_hash(price);
    if (LevelBucket_Empty(bucket)) {
        heap->prices[bucket->index = heap->size++] = price;
        OrderHeap_FixUp(heap, bucket->index);
    } else if (price > heap->prices[bucket->index]) {
        heap->prices[bucket->index] = price;
        OrderHeap_FixUp(heap, bucket->index);
    }
    Level_Add(LevelBucket_Get(bucket, price), order);
}

void
OrderHeap_Pop(struct OrderHeap *heap) {
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

struct OrderChunk *
OrderHeap_Peek(struct OrderHeap *heap) {
    return Level_Peek(LevelBucket_Peek(heap->buckets + LevelHeap_hash(heap->prices[0])));
}
