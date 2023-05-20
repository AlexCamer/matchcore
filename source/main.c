#include <stdio.h>
#include "level.h"
#include "levelheap.h"
#include "levelbucket.h"
#include "order.h"
#include "pool.h"
#include "types.h"

struct Test {
    struct Pool *pool;
    struct {
        void *root;
        void *best;
    } buckets[LEVEL_HEAP_CAPACITY];
    i32 prices[LEVEL_HEAP_CAPACITY];
    u16 index[LEVEL_HEAP_CAPACITY];
    usize size;
};

i32 main(void) {
    struct Pool pool;
    Pool_init(&pool, sizeof(struct Level), 128);
    struct LevelHeap heap;
    LevelHeap_init(&heap, &pool);
    struct LevelBucket bucket;
    LevelBucket_init(&bucket);
    LevelBucket_getOrAdd(&bucket, &heap, 12);
    LevelBucket_getOrAdd(&bucket, &heap, 13);
    struct Level *x = LevelBucket_getOrAdd(&bucket, &heap, 14);
    LevelBucket_remove(&bucket, x);
    printf("%d %d\n", bucket.root->price, bucket.best->price);
    printf("%d %d\n", bucket.root->balance, bucket.best->balance);
    printf("%lu %lu\n", sizeof(heap), sizeof(struct Test));
    Pool_destroy(&pool);
    return 0;
}
