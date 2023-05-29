#include "book.h"
#include "engine.h"
#include "order.h"

struct EngineBucket {
    char symbol[8];
    struct Book *book;
};

static struct Engine {

} engine;

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
