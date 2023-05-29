#pragma once

#include "orderheap.h"

struct Order;

struct Book {
    struct LevelHeap limitBuy;
    struct LevelHeap limitSell;
    struct LevelHeap stopBuy;
    struct LevelHeap stopSell;
};

void Book_Construct(struct Book *book);
void Book_Destruct(struct Book *book);
void Book_Add(struct Book *book, struct Order *order);
