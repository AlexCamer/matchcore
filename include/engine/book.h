#pragma once

#include "levelhashheap.h"

struct Order;

struct Book {
    struct LevelHashHeap limitBuy, limitSell, stopBuy, stopSell;
};

struct Book *Book_New(void);
void Book_Delete(struct Book *book);
void Book_Add(struct Book *book, struct Order *order);
