#include "test.h"

TEST(init) {
    ASSERT(1);
}

TEST(add) {
    ASSERT(1 == 1);
}

TEST(remove) {
    int x = 0;
    ASSERT(x == 0);
    ASSERT(x == 1);
}

TEST(peek) {
    ASSERT(1 != 2);
}

SUITE(Level) {
    RUN(init);
    RUN(add);
    RUN(remove);
    RUN(peek);
}
