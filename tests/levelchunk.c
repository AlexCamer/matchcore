#include "test.h"
#include "../src/levelchunk.c"

TEST(capacity) {
    ASSERT(LEVEL_CHUNK_CAPACITY_LOG2 < 8);
}

TEST(init) {
    struct LevelChunk c;
    c.level = (struct Level *) 1;
    c.next = (struct LevelChunk *) 1;
    c.prev = (struct LevelChunk *) 1;
    c.front = 1;
    c.back = 1;
    c.size = 1;
    for (u8 i = 0; i < LEVEL_CHUNK_CAPACITY; i++)
        c.queue[i] = LEVEL_CHUNK_CAPACITY;
    LevelChunk_init(&c, NULL);
    ASSERT(c.level == NULL);
    ASSERT(c.next == NULL);
    ASSERT(c.prev == NULL);
    ASSERT(c.front == 0);
    ASSERT(c.back == 0);
    ASSERT(c.size == 0);
    for (u8 i = 0; i < LEVEL_CHUNK_CAPACITY; i++)
        ASSERT(c.queue[i] == i);
}

TEST(add) {
    struct LevelChunk c;
    struct OrderData d;
    LevelChunk_init(&c, NULL);
    for (usize i = 0; i < LEVEL_CHUNK_CAPACITY; i++) {
        d.orderID = i;
        d.userID = i + 1;
        d.volume = i + 2;
        LevelChunk_add(&c, &d);
        ASSERT(c.orders[c.queue[i]].orderID == i);
        ASSERT(c.orders[c.queue[i]].userID == i + 1);
        ASSERT(c.orders[c.queue[i]].volume == i + 2);
        ASSERT(c.back == i + 1);
        ASSERT(c.size == i + 1);
    }
}

TEST(remove) {
    struct LevelChunk c;
    struct OrderData d;
    LevelChunk_init(&c, NULL);
    for (u8 i = 0; i < LEVEL_CHUNK_CAPACITY; i++) {
        LevelChunk_add(&c, &d);
    }
    for (u8 i = 0; i < LEVEL_CHUNK_CAPACITY; i++) {
        LevelChunk_remove(&c);
        ASSERT(c.front == i + 1);
        ASSERT(c.size == LEVEL_CHUNK_CAPACITY - i - 1);
    }
}

TEST(peek) {
    struct LevelChunk c;
    LevelChunk_init(&c, NULL);
    for (u8 i = 0; i < LEVEL_CHUNK_CAPACITY; i++) {
        c.front = i;
        ASSERT(LevelChunk_peek(&c) == &(c.orders[i]));
    }
}

TEST(wrap) {
    struct LevelChunk c;
    LevelChunk_init(&c, NULL);

}

SUITE(LevelChunk) {
    RUN(capacity);
    RUN(init);
    RUN(add);
    RUN(remove);
    RUN(peek);
    RUN(wrap);
}
