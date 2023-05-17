#include <ctype.h>
#include <string.h>
#include "engine.h"
#include "level.h"
#include "order.h"
#include "router.h"

struct RouterNode {
    char symbol[8];
    struct Engine *engine;
    struct RouterNode *children[ROUTER_CAPACITY];
};

bool symbolValid(char *symbol) {
    for (usize i = 0; i < 8 && symbol[i]; i++) {
        if (!isupper(symbol[i]))
            return false;
    }
    return true;
}

void Router_init(struct Router *r) {
    memset(r->children, 0, ROUTER_CAPACITY * sizeof(struct Router *));
}

void Router_destroy(struct Router *r) {

}

void Router_add(struct Router *r, char *symbol, struct Engine *e) {

}

struct Engine *Router_get(struct Router *r, char *symbol) {

}
