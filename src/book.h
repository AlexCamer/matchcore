#ifndef __LEVELS_H__
#define __LEVELS_H__

#include "types.h"

struct Book
{
    u32 trees[];
    u32 heap[];
    usize index[];
    usize size;
};

void
book_init(struct Book *book);

void
book_destroy(struct Book *book);

#endif /* __LEVELS_H__ */
