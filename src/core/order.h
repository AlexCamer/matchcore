#ifndef __CORE_ORDER_H__
#define __CORE_ORDER_H__

#include "util/primatives.h"

// order side
#define BUY  0
#define SELL 1

// order type
#define LMT  0 // limit
#define MKT  1 // market
#define STL  2 // stop-limit
#define STP  3 // stop
#define TSL  4 // trailing-stop-limit
#define TSP  5 // trailing-stop

// order time in force
#define AON  0 // all-or-none
#define FOK  1 // fill-or-kill
#define GTC  2 // good-till-cancelled
#define IOC  3 // immediate-or-cancel

struct order {
  u64 order_id;
  u64 user_id;
  u32 volume;
  i32 limit_price;
  i32 stop_price;
  u8 side;
  u8 type;
  u8 time_in_force;
  struct order *next_in_store;
  struct order *next_in_level;
  struct order *prev_in_level;
};

#endif
