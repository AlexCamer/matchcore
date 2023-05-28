#pragma once

#include "types.h"

enum OrderSide {
    BUY = 0,
    SELL = 1,
};

enum OrderType {
    MARKET = 0,
    LIMIT = 1,
    STOP = 2,
    CANCEL = 3
};

enum OrderTimeInForce {
    GTC = 0,
    AON = 1,
    IOC = 2,
    FOK = 3
};

struct Order {
    char symbol[8];
    enum OrderSide side:1;
    enum OrderType type:2;
    enum OrderTimeInForce timeInForce:2;
    u64 orderID;
    u32 userID;
    u32 volume;
    i32 price;
};

#define Order_Empty(order) ((order)->volume == 0)

void Order_Construct(struct Order *order);
