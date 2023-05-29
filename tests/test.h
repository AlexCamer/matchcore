#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "../source/types.h"

#define TEST(name) \
static void name##Func(void); \
static void (*name)(void) = &name##Func; \
static void name##Func(void)

#define ASSERT(expr) \
do { \
    if (!(expr)) { \
        fprintf(stderr, "Suite \x1B[31mfailed\x1B[0m @ Line: %d\n", __LINE__); \
        exit(1); \
    } \
} while (0)

#define SUITE(...) \
i32 main(void) { \
    void (*suite[])() = { __VA_ARGS__, NULL }; \
    for (usize i = 0; suite[i]; i++) \
        suite[i](); \
    printf("Suite \x1B[32mpassed\x1B[0m\n"); \
    return 0; \
}
