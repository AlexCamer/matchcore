#pragma once

#include "types.h"

enum RBColor {
    RED,
    BLACK
};

struct RBNode {
    enum RBColor color;
    struct RBNode *parent, *left, *right;
    u64 key;
};


