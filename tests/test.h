#ifndef __TEST_H__
#define __TEST_H__

#include <stdio.h>
#include <stdlib.h>

void handleFailure(char *suiteName, char *testName, char *expr) {
    fprintf(stderr, "Suite \x1B[31mfailed\x1B[0m: %s @ Test: %s @ Assert: %s\n",
            suiteName, testName, expr);
    exit(1);
}

#define TEST(name) \
static void name##Test(char *suiteName, char *testName)

#define ASSERT(expr) \
do { \
    if (!(expr)) \
        handleFailure(suiteName, testName, #expr); \
} while (0)

#define SUITE(name, ...) \
void suite(char *suiteName); \
int main(void) { \
    suite(#name); \
    printf("Suite \x1B[32mpassed\x1B[0m: " #name "\n"); \
    return 0; \
} \
void suite(char *suiteName)

#define RUN(name) name##Test(suiteName, #name)

#endif /* __TEST_H__ */
