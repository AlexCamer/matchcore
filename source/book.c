#include "book.h"
#include "level.h"
#include "../include/order.h"

static inline void
Book_addMarket(struct Book *book, struct Order *order) {
    struct LevelHeap *heap = (order->side == BUY) ? book->limitSell : book->limitBuy;
    while (!Order_empty(order) && !LevelHeap_empty(heap)) {
        struct Level *level = LevelHeap_peek(heap);
        Level_trade(level, order);
        if (Level_empty(level))
            LevelHeap_remove(level);
    }
}

static inline void
Book_addMarket(struct Book *book, struct Order *order) {
    while (!LevelHeap_empty(heap) && !Order_empty(order)) {
        struct Level *level = LevelHeap_peek(heap);
        Level_trade(level, order);
        if (Level_empty(level))
            LevelHeap_remove(level);
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