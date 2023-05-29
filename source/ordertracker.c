#include "ordertracker.h"

struct OrderTrackerNode {
    struct {
        void *next;
        u64 orderID;
    };
};
