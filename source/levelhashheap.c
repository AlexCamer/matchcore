#include "level.h"
#include "levelhashheap.h"

#define LevelHashHeap_Hash(price) ((usize) MOD_EXP2(price, LEVEL_HASH_HEAP_CAPACITY_LOG2))

static inline void
LevelHashHeap_Swap(struct LevelHashHeap *heap, usize i, usize j) {
    i32 iPrice = heap->prices[i];
    i32 jPrice = heap->prices[j];
    heap->prices[i] = jPrice;
    heap->prices[j] = iPrice;
    heap->index[LevelHashHeap_Hash(iPrice)] = j;
    heap->index[LevelHashHeap_Hash(jPrice)] = i;
}

static inline void
LevelHashHeap_FixUp(struct LevelHashHeap *heap, usize i) {
    for (usize parent = DIV2(i); heap->prices[i] > heap->prices[parent]; i = parent, parent = DIV2(parent))
        LevelHashHeap_Swap(heap, i, parent);
}

static inline void
LevelHashHeap_FixDown(struct LevelHashHeap *heap, usize i) {
    for (;;) {
        usize best = i, left = MUL2(i), right = left + 1;
        if (left < heap->size && heap->prices[left] > heap->prices[best])
            best = left;
        if (right < heap->size && heap->prices[right] > heap->prices[best])
            best = right;
        if (best == i)
            return;
        LevelHashHeap_Swap(heap, i, best);
        i = best;
    }
}

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
        heap->index[hash] = heap->size;
        LevelHashHeap_FixUp(heap, heap->size++);
    } else if (price > LevelTree_Peek(bucket)->price) {
        usize index = heap->index[hash];
        heap->prices[index] = price;
        LevelHashHeap_FixUp(heap, index);
    }
    return LevelTree_GetOrAdd(bucket, price);
}

void
LevelHashHeap_Remove(struct LevelHashHeap *heap, struct Level *level) {
    usize hash = LevelHashHeap_Hash(level->price), index = heap->index[hash];
    struct LevelTree *bucket = heap->buckets + hash;
    i32 price = LevelTree_Peek(bucket)->price;
    LevelTree_Remove(bucket, level);
    if (LevelTree_Empty(bucket)) {
        LevelHashHeap_Swap(heap, index, --(heap->size));
        LevelHashHeap_FixUp(heap, index);
        LevelHashHeap_FixDown(heap, index);
    } else if (price > LevelTree_Peek(bucket)->price) {
        heap->prices[index] = LevelTree_Peek(bucket)->price;
        LevelHashHeap_FixDown(heap, index);
    }
}

struct Level *
LevelHashHeap_Peek(struct LevelHashHeap *heap) {
    return LevelTree_Peek(heap->buckets + LevelHashHeap_Hash(heap->prices[0]));
}
