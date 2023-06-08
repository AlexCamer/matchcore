#pragma once

#include "symbol.h"
#include "types.h"

enum class OrderSide : bool {
    BUY,
    SELL
};

enum class OrderType : u8 {
    MARKET,
    LIMIT,
    STOP,
    CANCEL
};

enum class OrderTimeInForce : u8 {
    GTC,
    AON,
    IOC,
    FOK
};

class OrderBase {
public:
    OrderBase(u32 user_id, u32 volume);

    bool is_empty() { return m_volume == 0; }

    inline u64 get_order_id() { return m_order_id; }
    inline u32 get_user_id() { return m_user_id; }
    inline u32 get_volume() { return m_volume; }

    inline void set_order_id(u32 order_id) { m_order_id = order_id; }
    inline void set_volume(u32 volume) { m_volume = volume; }

private:
    u64 m_order_id;
    u32 m_user_id;
    u32 m_volume;
};

class Order : public OrderBase {
public:
    Order(u64 order_id, u32 user_id, u32 volume, OrderSide side, OrderType type, OrderTimeInForce time_in_force, u32 price);

    OrderSide get_side() { return m_side; }
    OrderType get_type() { return m_type; }
    OrderTimeInForce get_time_in_force() { return m_time_in_force; }
    u32 get_price() { return m_price; }

private:
    Symbol m_symbol;
    OrderSide m_side;
    OrderType m_type;
    OrderTimeInForce m_time_in_force;
    u32 m_price;
};
