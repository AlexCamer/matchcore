#pragma once

#include "levelhashheap.h.h"

struct Order;

struct Book {
    struct LevelHashHeap limitBuy;
    struct LevelHashHeap limitSell;
    struct LevelHashHeap stopBuy;
    struct LevelHashHeap stopSell;
};

struct Book *Book_New(void);
void Book_Delete(struct Book *book);
void Book_Add(struct Book *book, struct Order *order);
