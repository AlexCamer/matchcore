#include <stdio.h>
#include "../include/utility/types.h"
#include "../include/utility/pool.h"

i32 main(void) {
    struct Pool pool;
    Pool_Construct(&pool, 10, 10);
    Pool_Destruct(&pool);
    printf("Hey\n");
    return 0;
}
