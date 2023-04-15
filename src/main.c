#include <stdio.h>
#include "types.h"

i32
main(void)
{
    for (usize i = 0; i < 10; i++)
    {
        for (usize j = 0; j < 10; j++)
        {
            if (i == j)
                printf("1");
            else
                printf("0");
        }
    printf("\n");
    }
}
