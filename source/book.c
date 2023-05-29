#include "book.h"
#include "level.h"
#include "order.h"

void
book_Construct(struct Book *book) {
    LevelHeap_Construct(book->limitBuy);
    LevelHeap_Construct(book->limitSell);
    LevelHeap_Construct(book->stopBuy);
    LevelHeap_Construct(book->stopSell);
}

void
book_Destruct(struct Book *book) {
    LevelHeap_Destruct(book->limitBuy);
    LevelHeap_Destruct(book->limitSell);
    LevelHeap_Destruct(book->stopBuy);
    LevelHeap_Destruct(book->stopSell);
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
        case CANCEL:
            break;
    }
}