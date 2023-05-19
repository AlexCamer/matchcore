#pragma once

#include "macros.h"
#include "types.h"

#define BOOK_CAPACITY_LOG2 12
#define BOOK_CAPACITY EXP2(BOOK_CAPACITY_LOG2)

struct Level;
struct Order;

struct Book {
    struct Level *tree[BOOK_CAPACITY];
    struct Level *best[BOOK_CAPACITY];
    i32 heap[BOOK_CAPACITY];
    u16 index[BOOK_CAPACITY];
    usize size;
};

void Book_init(struct Book *b);
void Book_destroy(struct Book *b);
void Book_add(struct Book *b, struct Order *o);
void Book_remove(struct Book *b);
struct Level *Book_peek(struct Book *b);
