#pragma once

#include <stdio.h>
#include <stdlib.h>

void handleFailure(char *suiteName, char *testName, char *expr) {
    fprintf(stderr, "Suite \x1B[31mfailed\x1B[0m: %s @ Test: %s @ Assert: %s\n",
            suiteName, testName, expr);
    exit(1);
}

#define TEST(name) \
static void name##FuncHelper(char *suiteName, char *testName); \
static void name##Func(char *suiteName) { \
    name##FuncHelper(suiteName, #name); \
} \
static void (*name)(char *) = &name##Func; \
static void name##FuncHelper(char *suiteName, char *testName)

#define ASSERT(expr) \
do { \
    if (!(expr)) \
        handleFailure(suiteName, testName, #expr); \
} while (0)

#define SUITE(name, ...) \
int main(int argc, char **argv) { \
    void (*suite[])(char *) = { __VA_ARGS__, NULL }; \
    for (unsigned int i = 0; suite[i]; i++) { \
        suite[i](#name); \
    } \
    printf("Suite \x1B[32mpassed\x1B[0m: " #name "\n"); \
    return 0; \
}
