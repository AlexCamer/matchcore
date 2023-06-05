#pragma once

#include <stdint.h>

struct Level {
    uint32_t price;
    uint64_t volume;
    void* front;
    void* back;
};
