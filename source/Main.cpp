#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "HashHeap.h"
#include "Level.h"

#define SEED 52387301
#define CAPACITY (1 << 8)
#define INSERTS (1 << 24)
#define RANGE 4096

int main(void) {
    srand(SEED);
    HashHeap<uint32_t, Level*, std::greater<uint32_t>> heaps[CAPACITY];
    clock_t start = clock();
    for (size_t i = 0; i < INSERTS; i++) {
        auto& heap1 = heaps[rand() % CAPACITY];
        uint32_t price = rand() % RANGE;
        if (!heap1.contains(price))
            heap1.insert(price, nullptr);
        if ((i % 8) == 0) {
            auto& heap2 = heaps[rand() % CAPACITY];
            if (heap2.empty())
                continue;
            heap2.pop();
        }
    }
    clock_t time = clock() - start;
    int milliseconds = time * 1000 / CLOCKS_PER_SEC;
    std::cout << "Time: " << milliseconds << "ms\n";
    return 0;
}