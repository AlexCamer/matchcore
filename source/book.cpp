#include "book.h"

void add(Order& order) {
    switch (order.get_time_in_force()) {
        case MARKET:
            break;
        case LIMIT:
            break;
        case STOP:
            break;
    }
}
