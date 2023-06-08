#include <string.h>
#include "../../include/engine/trade.h"

void
Trade_construct(struct Trade *trade,
                char symbol[8],
                u64 buyOrderID,
                u64 sellOrderID,
                u32 buyUserID,
                u32 sellUserID,
                u32 volume,
                i32 price)
{
    trade->buyOrderID = buyOrderID;
    trade->sellOrderID = sellOrderID;
    trade->buyUserID = buyUserID;
    trade->sellUserID = sellUserID;
    trade->volume = volume;
    trade->price = (u32) price;
    memcpy(trade->symbol, symbol, 8);
}
