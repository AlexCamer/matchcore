#include "book.h"

static inline void
book_heap_swap(struct Book *book, usize i, usize j)
{
    i32 price_i = book->heap[i];
    i32 price_j = book->heap[j];
    book->index[MOD2(price_i, BOOK_CAPACITY_LOG2)] = j;
    book->index[MOD2(price_j, BOOK_CAPACITY_LOG2)] = i;
    book->heap[i] = price_j;
    book->heap[j] = price_i;
}

static void
book_heap_fix_up(struct Book *book, usize i)
{
    u32 *heap = book->heap;
    usize parent = DIV2(i);
    for (; heap[i] > heap[parent]; i = parent, parent = DIV2(parent))
        book_heap_swap(book, i, parent);
}

static void
book_heap_fix_down(struct Book *book, usize i)
{
    u32 *heap = book->heap;
    for (;;)
    {
        usize best = i, left = MUL2(i), right = left + 1;
        if (left < book->size && heap[left] > heap[best])
            best = left;
        if (right < book->size && heap[right] > heap[best])
            best = right;
        if (best == i)
            break;
        book_heap_swap(book, i, best);
        i = best;
    }
}

void
book_init(struct Book *book)
{

}

void
book_destroy(struct Book *book)
{

}
