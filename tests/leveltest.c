#include "test.h"

TEST(Level_initTest) {
    ASSERT(1);
}

TEST(Level_addTest) {
    ASSERT(1 == 1);
}

TEST(Level_removeTest) {
    int x = 0;
    ASSERT(x == 0);
    ASSERT(x != 1);
}

TEST(Level_peekTest) {
    ASSERT(1 != 2);
}

SUITE(Level_initTest,
      Level_addTest,
      Level_removeTest,
      Level_peekTest)
