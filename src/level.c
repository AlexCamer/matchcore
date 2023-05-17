#include "level.h"
// #include "orderchunk.h"

void Level_init(struct Level *l, struct Levels *ls, enum LevelColor lc, i32 price) {
    l->levels = ls;
    l->parent = NULL;
    l->left = NULL;
    l->right = NULL;
    l->front = NULL;
    l->back = NULL;
    l->color = lc;
    l->volume = 0;
    l->price = price;
}

void Level_add(struct Level *l, struct Order *o) {

}

void Level_remove(struct Level *l) {

}

void Level_peek(struct Level *l) {

}
