#pragma once

struct Trade {
    char symbol[8];
    u64 buyOrderID;
    u64 sellOrderID;
    u32 buyUserID;
    u32 sellUserID;
    u32 volume;
    u32 price;
};

void Trade_handle(struct Trade *trade);
void Trade_construct(struct Trade *trade,
                     char symbol[8],
                     u64 buyOrderID,
                     u64 sellOrderID,
                     u32 buyUserID,
                     u32 sellUserID,
                     u32 volume,
                     i32 price);
