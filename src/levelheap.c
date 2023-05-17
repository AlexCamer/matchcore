#include <string.h>
#include "levelheap.h"
#include "leveltree.h"
#include "pool.h"

#define LevelHeap_hash(price) ((usize) MOD_EXP2(price, LEVELS_CAPACITY_LOG2))

static inline void LevelHeap_swap(struct LevelHeap *lh, usize i, usize j) {
    i32 iPrice = lh->prices[i];
    i32 jPrice = lh->prices[j];
    lh->index[LevelHeap_hash(iPrice)] = j;
    lh->index[LevelHeap_hash(jPrice)] = i;
    lh->prices[i] = jPrice;
    lh->prices[j] = iPrice;
}

static void LevelHeap_fixUp(struct LevelHeap *lh, usize i) {
    i32 *prices = lh->prices;
    usize parent = DIV2(i);
    for (; prices[i] > prices[parent]; i = parent, parent = DIV2(parent))
        LevelHeap_swap(ls, i, parent);
}

static void LevelHeap_fixDown(struct LevelHeap *lh, usize i) {
    i32 *prices = lh->prices;
    for (;;) {
        usize best = i, left = MUL2(i), right = left + 1;
        if (left < lh->size && prices[left] > prices[best])
            best = left;
        if (right < lh->size && prices[right] > prices[best])
            best = right;
        if (best == i)
            return;
        LevelHeap_swap(lh, i, best);
        i = best;
    }
}

void LevelHeap_init(struct LevelHeap *lh) {
    memset(lh->trees, 0, sizeof(lh->trees));
    lh->size = 0;
}

struct Level *LevelHeap_getOrAdd(struct LevelHeap *lh, i32 price) {
    usize hash = Levels_hash(price), index = (usize) lh->index[hash];
    struct LevelTree *lt = ls->trees + hash;
    if (LevelTree_empty(lt)) {
        lh->prices[lh->size] = price;
        lh->index[hash] = lh->size;
        LevelHeap_fixUp(lh, lh->size++);
    } else {
        if (price == lh->prices[index])
            return LevelTree_peek(lt);
        if (price > lh->prices[index]) {
            lh->prices[index] = price;
            LevelHeap_fixUp(ph, index);
        }
    }
    return LevelTree_getOrAdd(lt, lh, price);
}

struct Level *LevelHeap_peek(struct LevelHeap *lh) {
    return lh->trees[MOD_EXP2(lh->heap[0], LEVELS_CAPACITY_LOG2)].best;
}

void LevelHeap_remove(struct LevelHeap *lh, struct Level *l) {
    usize hash = Levels_hash(price), index = (usize) lh->index[hash];
    struct LevelTree *lt = ls->trees + hash;
    LevelTree_remove(lt, l);
    if (LevelTree_empty(lt)) {
        LevelHeap_swap(lh, index, --(lh->size));
        LevelHeap_fixUp(lh, index);
        LevelHeap_fixDown(lh, index);
    } else if (LevelTree_peek(lt)->price < lh->prices[index]) {
        lh->prices[index] = price;
        LevelHeap_fixDown(lh, index);
    }
}

void LevelHeap_pop(struct LevelHeap *lh) {
    usize hash = Levels_hash(lh->prices[0]);
    struct LevelTree *lt = ls->trees + hash;
    LevelTree_pop(lt);
    if (LevelTree_empty(lt))
        LevelHeap_swap(lh, 0, --(lh->size));
    else
        lh->prices[0] = LevelTree_peek(lt)->price;
    LevelHeap_fixDown(lh, 0);
}
