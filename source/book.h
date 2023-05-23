#pragma once

#include "levelheap.h"

struct Order;

struct Book {
    struct LevelHeap limitBuy;
    struct LevelHeap limitSell;
    struct LevelHeap stopBuy;
    struct LevelHeap stopSell;
};

void Book_construct(struct Book *book);
void Book_destruct(struct Book *book);
void Book_add(struct Book *book, struct Order *order);
