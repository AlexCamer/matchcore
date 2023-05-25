#include "../include/engine.h"
#include "../include/order.h"

static struct {

} Engine;

__attribute__((constructor))
static inline void
Engine_construct(void) {

}

__attribute__((destructor))
static inline void
Engine_destruct(void) {

}

void
Engine_add(struct Order *order) {

}
