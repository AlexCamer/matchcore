#include "level.h"
// #include "orderchunk.h"

void Level_init(struct Level *level, struct LevelHeap *heap, i32 price) {
    level->heap = heap;
    level->parent = NULL;
    level->left = NULL;
    level->right = NULL;
    level->front = NULL;
    level->back = NULL;
    level->balance = 0;
    level->volume = 0;
    level->price = price;
}

void Level_add(struct Level *level, struct Order *order) {

}

void Level_remove(struct Level *level) {

}

void Level_peek(struct Level *level) {

}
