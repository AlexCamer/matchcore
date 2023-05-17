#include <stdio.h>
#include "level.h"
#include "order.h"
#include "types.h"

i32 main(void) {
    struct OrderChunk chunk;
    order_chunk_init(&chunk);
    for (usize i = 0; i < ORDER_CHUNK_CAPACITY; i++) {
        u64 queue = MOD2(chunk.queue >> (i * ORDER_CHUNK_CAPACITY_LOG2), ORDER_CHUNK_CAPACITY_LOG2);
        printf("%llu ",  queue);
    }
    printf("\n%lu\n", order_chunk_full(&chunk));
    return 0;
}
