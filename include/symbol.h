#pragma once

#include "stdint.h"

class Symbol {
public:
    Symbol();
    uint64_t to_uint64();

private:
    char string[8];
};
