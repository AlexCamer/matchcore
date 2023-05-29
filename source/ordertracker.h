#pragma once

struct OrderChunk;
struct OrderTrackerNode;

struct OrderTracker {
    struct OrderTrackerNode *root;
    u64 orderID;
};

void OrderTracker_Construct();
void OrderTracker_Destruct();
void OrderTracker_Add(u64 orderID, struct OrderChunk *chunk);
void OrderTracker_Remove(u64 orderID);
struct OrderChunk *OrderTracker_Get(u64 orderID);
