#include "level.h"
#include "levelheap.h"
#include "leveltree.h"
#include "pool.h"

static void LevelTree_rotateLeft(struct LevelTree *lt, struct Level *l) {
    struct Level *right = l->right;
    l->right = right->left;
    right->left = l;
    right->parent = l->parent;
    l->parent = right;
    if (l->right)
        l->right->parent = l;
    if (!(right->parent))
        lt->root = right;
    else if (right->parent->left == l)
        right->parent->left = right;
    else
        right->parent->right = right;
}

static void LevelTree_rotateRight(struct LevelTree *lt, struct Level *l) {
    struct Level *left = l->left;
    l->left = left->right;
    left->right = l;
    left->parent = l->parent;
    l->parent = left;
    if (l->left)
        l->left->parent = l;
    if (!(left->parent))
        lt->root = left;
    else if (left->parent->left == l)
        left->parent->left = left;
    else
        left->parent->right = left;
}

static void LevelTree_removeFixUp(struct LevelTree *lt, struct Level *l) {
    while (l->parent && l->color == BLACK) {
        if (l->parent->left == l) {
            struct Level *sibling = l->parent->right;
            if (sibling->color == RED) {

            }
        } else {
            struct Level *sibling = l->parent->left;
        }
    }
    lt->root->color = BLACK;
}

static void LevelTree_addFixUp(struct LevelTree *lt, struct Level *l) {
    while (l->parent && l->parent->color == RED) {
        struct Level *parent = l->parent, *grandParent = l->parent->parent;
        if (grandParent->left == parent) {
            struct Level *uncle = grandParent->right;
            if (uncle->color == RED) {
                parent->color = BLACK;
                grandParent->color = RED;
                uncle->color = BLACK;
                *l = grandParent;
            } else {
                if (parent->right == l) {
                    LevelTree_rotateLeft(lt, parent);
                    l = parent;
                }
                grandParent->color = BLACK;
                grandParent->parent->color = RED;
                LevelTree_rotateRight(lt, grandParent->parent);
            }
        } else {
            struct Level *uncle = grandParent->left;
            if (uncle->color == RED) {
                parent->color = BLACK;
                grandParent->color = RED;
                uncle->color = BLACK;
                *l = grandParent;
            } else {
                if (parent->left == l) {
                    LevelTree_rotateRight(lt, parent);
                    l = parent;
                }
                grandParent->color = BLACK;
                grandParent->parent->color = RED;
                LevelTree_rotateLeft(lt, grandParent->parent);
            }
        }
    }
    lt->root->color = BLACK;
}

static inline struct Level **LevelTree_get(struct LevelTree *lt, i32 price) {
    if (price == lt->best->price)
        return &(lt->best);
    struct Level **l = &(lt->best->left);
    if (price < lt->best->price) {
        l = &(lt->root);
        while (*l) {
            if (price > (*l)->price)
                l = &((*l)->left);
            else if (price < (*l)->price)
                l = &((*l)->right);
            else
                break;
        }
    }
    return l;
}

void LevelTree_init(struct LevelTree *lt) {
    lt->root = NULL;
    lt->best = NULL;
}

struct Level *LevelTree_getOrAdd(struct LevelTree *lt, struct Levels *ls, i32 price) {
    if (lt->root) {
        struct Level **l = LevelTree_get(lt, price);
        if (!*l) {
            *l = (struct Level *) Pool_alloc(ls->pool);
            Level_init(*l, ls, RED, price);
            LevelTree_addFixUp(lt, *l);
        }
        return *l;
    } else {
        lt->root = lt->best = (struct Level *) Pool_alloc(ls->pool);
        Level_init(lt->root, ls, BLACK, price);
        return lt->root;
    }
}

/*
 * if (lt->best == l) {
        if (lt->best->right) {
            lt->best = lt->best->right;
            while (lt->best->left)
                lt->best = lt->best->left;
            PriceHeap_replace(ph, hash, lt->best->price);
        } else if (lt->best->parent) {
            lt->best = lt->best->parent;
            PriceHeap_replace(ph, hash, lt->best->price);
        } else {
            lt->root = NULL;
            lt->best = NULL;
            PriceHeap_remove(ph, hash);
        }
    }
 */

void LevelTree_remove(struct LevelTree *lt, struct Level *l) {
    struct Level *successor;
    if (l->left && l->right) {

    } else {

    }
}

void LevelTree_pop(struct LevelTree *lt) {
    LevelTree_remove(lt, lt->best);
}
