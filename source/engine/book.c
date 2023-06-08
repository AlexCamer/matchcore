#include "../../include/engine/book.h"
#include "../../include/engine/level.h"
#include "../../include/engine/order.h"
#include "../../include/utility/pool.h"

static struct Pool bookPool;

__attribute__((constructor))
static inline void
Book_ConstructPool(void) {
    Pool_Construct(&bookPool, sizeof(struct Book), BOOK_POOL_ELEMENTS_PER_BLOCK);
}

__attribute__((destructor))
static inline void
Book_DestructPool(void) {
    Pool_Destruct(&bookPool);
}

static inline void
Book_Construct(struct Book *book) {
    LevelHeap_Construct(book->limitBuy);
    LevelHeap_Construct(book->limitSell);
    LevelHeap_Construct(book->stopBuy);
    LevelHeap_Construct(book->stopSell);
}

static inline void
Book_Destruct(struct Book *book) {
    LevelHeap_Destruct(book->limitBuy);
    LevelHeap_Destruct(book->limitSell);
    LevelHeap_Destruct(book->stopBuy);
    LevelHeap_Destruct(book->stopSell);
}

static inline void
Book_AddMarket(struct Book *book, struct Order *order) {
    struct LevelHashHeap *heap;
    while (!LevelHashHeap_Empty(heap) && !Order_Empty(order)) {
        struct Level *level = LevelHashHeap_Peek(heap);

    }
}

static inline void
Book_AddLimit(struct Book *book, struct Order *order) {

}

static inline void
Book_AddStop(struct Book *book, struct Order *order) {

}

struct Book *Book_New(void) {
    struct Book *book = (struct Book *) Pool_Malloc(bookPool);
    Book_Construct(book);
    return book;
}

void Book_Delete(struct Book *book) {
    Book_Destruct(book);
    Pool_Free(bookPool, (void *) book);
}

void
Book_Add(struct Book *book, struct Order *order) {
    switch (order->type) {
        case MARKET:
            Book_AddMarket(book, order);
            break;
        case LIMIT:
            Book_AddLimit(book, order);
            break;
        case STOP:
            Book_AddLimit(book, order);
            break;
    }
}
