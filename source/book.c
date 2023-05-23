#include "book.h"
#include "level.h"
#include "macros.h"
#include "order.h"

static inline void
Book_addMarket(struct Book *book, struct Order *order) {
    while (!LevelHeap_empty(book->limitBuy)) {
        struct Level *level = LevelHeap_peek(book->limitBuy);
        while (!Level_empty(level)) {


            Level_pop(level);
        }
        LevelHeap_pop(heap);
    }
}

void
book_construct(struct Book *book) {
    LevelHeap_construct(book->limitBuy);
    LevelHeap_construct(book->limitSell);
    LevelHeap_construct(book->stopBuy);
    LevelHeap_construct(book->stopSell);
}

void
book_destruct(struct Book *book) {
    LevelHeap_destruct(book->limitBuy);
    LevelHeap_destruct(book->limitSell);
    LevelHeap_destruct(book->stopBuy);
    LevelHeap_destruct(book->stopSell);
}

void
Book_add(struct Book *book, struct Order *order) {
    switch (order->type) {
        case MARKET:
            break;
        case LIMIT:
            break;
        case STOP:
            break;
    }
}