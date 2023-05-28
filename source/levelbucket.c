#include "level.h"
#include "leveltree.h"
#include "macros.h"

static inline void
LevelTree_SetLeft(struct Level *level, struct Level *left) {
    level->left = left;
    if (left != NULL)
        left->parent = level;
}

static inline void
LevelTree_SetRight(struct Level *level, struct Level *right) {
    level->right = right;
    if (right != NULL)
        right->parent = level;
}

static inline void
LevelTree_SetParent(struct Level *level, struct Level *parent) {
    level->parent = parent;
    if (parent != NULL)
        (level->price < parent->price) ? LevelTree_SetLeft(parent, level)
                                       : LevelTree_SetRight(parent, level);
}

static inline void
LevelTree_RotateLeft(struct LevelTree *tree, struct Level *level) {
    struct Level *right = level->right;
    LevelTree_SetParent(right, level->parent);
    LevelTree_SetRight(level, right->left);
    LevelTree_SetLeft(right, level);
    level->balance -= MAX(right->balance, 0) + 1;
    right->balance += MIN(level->balance, 0) - 1;
    if (tree->root == level)
        tree->root = right;
}

static inline void
LevelTree_RotateRight(struct LevelTree *tree, struct Level *level) {
    struct Level *left = level->left;
    LevelTree_SetParent(left, level->parent);
    LevelTree_SetLeft(level, left->right);
    LevelTree_SetRight(left, level);
    level->balance -= MIN(left->balance, 0) - 1;
    left->balance += MAX(level->balance, 0) + 1;
    if (tree->root == level)
        tree->root = left;
}

static inline void
LevelTree_Unlink(struct Level *level) {
    struct Level *parent = level->parent;
    if (parent != NULL)
        (parent->left == level) ? LevelTree_SetLeft(parent, NULL)
                                : LevelTree_SetRight(parent, NULL);
    Level_Delete(level);
}

static inline void
LevelTree_SwapLeft(struct Level *level, struct Level *successor) {
    struct Level *temp = level->left;
    LevelTree_SetLeft(level, successor->left);
    LevelTree_SetLeft(successor, temp);
}

static inline void
LevelTree_SwapRight(struct Level *level, struct Level *successor) {
    struct Level *temp = level->right;
    LevelTree_SetRight(level, successor->right);
    LevelTree_SetRight(successor, temp);
}

static inline void
LevelTree_Swap(struct LevelTree *tree, struct Level *level, struct Level *successor) {
    if (tree->root == level)
        tree->root = successor;
    if (level->left == successor) {
        LevelTree_SetParent(successor, level->parent);
        LevelTree_SetLeft(level, successor->left);
        LevelTree_SetLeft(successor, level);
        LevelTree_SwapRight(level, successor);
    } else if (level->right == successor) {
        LevelTree_SetParent(successor, level->parent);
        LevelTree_SetRight(level, successor->right);
        LevelTree_SetRight(successor, level);
        LevelTree_SwapLeft(level, successor);
    } else {
        struct Level *temp = level->parent;
        LevelTree_SetParent(level, successor->parent);
        LevelTree_SetParent(successor, temp);
        LevelTree_SwapLeft(level, successor);
        LevelTree_SwapRight(level, successor);
    }
    i8 temp = level->balance;
    level->balance = successor->balance;
    successor->balance = temp;
}

static inline void
LevelTree_DestructRecursive(struct Level *level) {
    if (level == NULL)
        return;
    LevelTree_DestructRecursive(level->left);
    LevelTree_DestructRecursive(level->right);
    Level_Delete(level);
}

static inline void
LevelTree_AddFixUp(struct LevelTree *tree, struct Level *level) {
    for (struct Level *parent = level->parent; parent != NULL; level = parent, parent = parent->parent) {
        if (parent->left == level) {
            switch (--(parent->balance)) {
                case -2:
                    if (level->balance == 1)
                        LevelTree_RotateLeft(tree, level);
                    LevelTree_RotateRight(tree, parent);
                case 0:
                    return;
            }
        } else {
            switch (++(parent->balance)) {
                case 2:
                    if (level->balance == -1)
                        LevelTree_RotateRight(tree, level);
                    LevelTree_RotateLeft(tree, parent);
                case 0:
                    return;
            }
        }
    }
}

static inline void
LevelTree_RemoveFixUp(struct LevelTree *tree, struct Level *level) {
    for (struct Level *parent = level->parent; parent != NULL; level = parent, parent = parent->parent) {
        if (parent->left == level) {
            switch (++(parent->balance)) {
                case 2:
                    if (level->balance == -1)
                        LevelTree_RotateRight(tree, level);
                    LevelTree_RotateLeft(tree, parent);
                case 1:
                    return;
            }
        } else {
            switch (--(parent->balance)) {
                case -2:
                    if (level->balance == 1)
                        LevelTree_RotateLeft(tree, level);
                    LevelTree_RotateRight(tree, parent);
                case -1:
                    return;
            }
        }
    }
}

void
LevelTree_Construct(struct LevelTree *tree) {
    tree->root = NULL;
    tree->best = NULL;
}

void
LevelTree_Destruct(struct LevelTree *tree) {
    LevelTree_DestructRecursive(tree->root);
}
#include <stdio.h>
struct Level *
LevelTree_GetOrAdd(struct LevelTree *tree, i32 price) {
    // return Level_New(price);
    if (tree->root == NULL)
        return tree->root = tree->best = Level_New(price);
    if (price == tree->best->price)
        return tree->best;
    if (price > tree->best->price) {
        LevelTree_SetRight(tree->best, Level_New(price));
        LevelTree_AddFixUp(tree, tree->best->right);
        return tree->best = tree->best->right;
    }
    struct Level *level = tree->root;
    int i = 0;
    for (;; i++) {
        if (price == level->price) {
            //printf("%d\n", i);
            return level;
        }
        if (price < level->price) {
            if (level->left == NULL)
                break;
            level = level->left;
        } else {
            if (level->right == NULL)
                break;
            level = level->right;
        }
    }
    // printf("%d\n", i);
    struct Level *new = Level_New(price);
    LevelTree_SetParent(new, level);
    LevelTree_AddFixUp(tree, new);
    return new;
}

void
LevelTree_Remove(struct LevelTree *tree, struct Level *level) {
    if (tree->best == level) {
        if (level->parent == level->left) {
            tree->root = tree->best = NULL;
            LevelTree_Unlink(level);
            return;
        }
        if (level->left != NULL)
            LevelTree_Swap(tree, level, level->left);
        tree->best = level->parent;
        LevelTree_RemoveFixUp(tree, level);
        LevelTree_Unlink(level);
        return;
    }
    for (;;) {
        if (level->left == level->right) {
            LevelTree_RemoveFixUp(tree, level);
            LevelTree_Unlink(level);
            return;
        } else {
            struct Level *successor = (level->left != NULL) ? level->left : level->right;
            while (successor->right != NULL)
                successor = successor->right;
            LevelTree_Swap(tree, level, successor);
        }
    }
}
