#pragma once

struct Transaction {
    char *symbol;
    u32 buyerID;
    u32 sellerID;
    u32 volume;
    u32 price;
};
