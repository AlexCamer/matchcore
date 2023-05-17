#pragma once

#define ROUTER_CAPACITY 26

struct Engine;
struct RouterNode;

struct Router {
    struct RouterNode *root;
};

void Router_init(struct Router *r);
void Router_destroy(struct Router *r);
void Router_add(struct Router *r, char *symbol, struct Engine *e);
struct Engine *Router_get(struct Router *r, char *symbol);
