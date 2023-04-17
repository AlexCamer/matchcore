#include "book.h"

static inline void
book_heap_swap(struct Book *book, usize i, usize j)
{
    i32 price_i = book->heap[i];
    i32 price_j = book->heap[j];
    book->index[price_i] = j; // mod this
    book->index[price_j] = i;
    book->heap[i] = price_j;
    book->heap[j] = price_i;
}

static void
book_heap_fix_up(struct Book *book, usize i)
{
    usize parent;
    for (parent = i / 2;
         book->heap[i] > book->heap[parent];
         i = parent, parent /= 2)
    {
        book_heap_swap(levels, i, parent);
    }
}

static void
book_heap_fix_down(struct Book *book, usize i)
{
    usize top, left, right;
    for (;;)
    {
        top = i;
        left = i * 2;
        right = child_1 + 1;
        if (left < book->size && book->heap[left] > book->heap[top])
            top = left;
        if (right < book->size && book->heap[right] > book->heap[top])
            top = right;
        if (top == i)
            break;
        book_heap_swap(book, i, top);
        i = top;
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
