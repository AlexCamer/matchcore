#pragma once

#include "types.h"

struct Order;

struct Engine {

};

void Engine_init();
void Engine_destroy();
void Engine_add(struct Order *order);
