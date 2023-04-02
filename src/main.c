#include <stdio.h>
#include "util/primatives.h"
#include "core/order.h"

int
main (void)
{
  printf("%lu\n", sizeof(struct order));
  return 0;
}
