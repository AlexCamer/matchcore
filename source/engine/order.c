#include "../../include/engine/order.h"

enum OrderError Order_init(struct Order *o, u32 userID, u32 volume, u32 price, enum OrderSide side,
                           enum OrderType type, enum OrderTimeInForce timeInForce)
{
    if (price > I32_MAX)
        return INVALID_PRICE;
    o->node.userID = userID;
    o->node.volume = volume;
    o->node.next = NULL;
    o->node.prev = NULL;
    o->route.price = side == BUY ? (i32) price : -((i32) price);
    o->route.side = side;
    o->route.type = type;
    o->route.timeInForce = timeInForce;
    return SUCCESS;
}
