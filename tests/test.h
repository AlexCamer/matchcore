#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "mock/alloc.c"
#include "../source/types.h"

static char *errorMessage = "Suite \x1B[31mfailed\x1B[0m: %s @ Test: %s @ Assert: %s\n";

static void (*before)(void);
static void (*after)(void);

#define BEFORE \
static void beforeFunc(void); \
static void (*before)(void) = &beforeFunc; \
static void beforeFunc(void) \

#define AFTER \
static void afterFunc(void); \
static void (*after)(void) = &afterFunc; \
static void afterFunc(void) \

#define TEST(name) \
static void name##FuncHelper(char *suiteName, char *testName); \
static void name##Func(char *suiteName) { \
    name##FuncHelper(suiteName, #name); \
} \
static void (*name)(char *) = &name##Func; \
static void name##FuncHelper(char *suiteName, char *testName)

#define ASSERT(expr) \
do { \
    if (!(expr)) { \
        fprintf(stderr, errorMessage, suiteName, testName, #expr); \
        exit(1); \
    } \
} while (0)

#define SUITE(name, ...) \
i32 main(int argc, char *argv[]) { \
    void (*suite[])(char *) = { __VA_ARGS__, NULL }; \
    for (usize i = 0; suite[i]; i++) { \
        if (before != NULL) \
            before(); \
        suite[i](name); \
        if (after != NULL) \
            after(); \
    } \
    printf("Suite \x1B[32mpassed\x1B[0m: %s\n", name); \
    return 0; \
}
