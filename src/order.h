#ifndef __ORDER_H__
#define __ORDER_H__

#include "types.h"

/* order side */
#define BUY  0
#define SELL 1

/* order type                        */
#define LMT 0 /* limit               */
#define MKT 1 /* market              */
#define STL 2 /* stop-limit          */
#define STP 3 /* stop                */
#define TSL 4 /* trailing-stop-limit */
#define TSP 5 /* trailing-stop       */

/* order time in force               */
#define AON 0 /* all-or-none         */
#define FOK 1 /* fill-or-kill        */
#define GTC 2 /* good-till-cancelled */
#define IOC 3 /* immediate-or-cancel */

struct OrderData
{
    u64 order_id;
    i32 user_id;
    u32 volume;
};

struct OrderRoute
{
    i32 limit_price;
    i32 stop_price;
    u8 side;
    u8 type;
    u8 time_in_force;
};

struct Order
{
    struct OrderData data;
    struct OrderRoute route;
};

#endif /* __ORDER_H__ */
