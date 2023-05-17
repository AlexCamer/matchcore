#pragma once

#include "types.h"

enum OrderSide {
    BUY = 0,
    SELL = 1,
};

enum OrderType {
    MARKET = 0,
    LIMIT = 1,
    STOP = 2
};

enum OrderTimeInForce {
    GTC = 0,
    AON = 1,
    IOC = 2,
    FOK = 3
};

struct Order {
    enum OrderSide:1 side;
    enum OrderType:2 type;
    enum OrderTimeInForce:2 timeInForce;
    Symbol symbol;
    OrderID orderID;
    UserID userID;
    Volume volume;
    Price price;
};

void Order_initMarket(struct Order *o,
                      enum OrderSide side
                      enum OrderTimeInForce timeInForce,
                      Symbol symbol;
                      UserID userID,
                      Volume volume);

void Order_initLimit(struct Order *o,
                     enum OrderSide side,
                     enum OrderTimeInForce timeInForce,
                     Symbol symbol,
                     UserID userID,
                     Volume volume,
                     Price price);

void Order_initStop(struct Order *o,
                    enum OrderSide side,
                    enum OrderTimeInForce timeInForce,
                    Symbol symbol,
                    UserID userID,
                    Volume volume,
                    Price price);
