#pragma once

#include "book.h"

struct Order;

struct Engine {
    struct Book limitBuyBook;
    struct Book limitSellBook;
    struct Book stopBuyBook;
    struct Book stopSellBook;
};

void Engine_init(struct Engine *e);
void Engine_add(struct Engine *e, struct Order *o);
