#include "level.h"
#include "levelbucket.h"
#include "macros.h"

static inline void
LevelBucket_SetLeft(struct Level *level, struct Level *left) {
    level->left = left;
    if (left != NULL)
        left->parent = level;
}

static inline void
LevelBucket_SetRight(struct Level *level, struct Level *right) {
    level->right = right;
    if (right != NULL)
        right->parent = level;
}

static inline void
LevelBucket_SetParent(struct Level *level, struct Level *parent) {
    level->parent = parent;
    if (parent != NULL)
        (level->price < parent->price) ? LevelBucket_SetLeft(parent, level)
                                       : LevelBucket_SetRight(parent, level);
}

static inline void
LevelBucket_RotateLeft(struct LevelBucket *tree, struct Level *level) {
    struct Level *right = level->right;
    LevelBucket_SetParent(right, level->parent);
    LevelBucket_SetRight(level, right->left);
    LevelBucket_SetLeft(right, level);
    level->balance -= MAX(right->balance, 0) + 1;
    right->balance += MIN(level->balance, 0) - 1;
    if (tree->root == level)
        tree->root = right;
}

static inline void
LevelBucket_RotateRight(struct LevelBucket *tree, struct Level *level) {
    struct Level *left = level->left;
    LevelBucket_SetParent(left, level->parent);
    LevelBucket_SetLeft(level, left->right);
    LevelBucket_SetRight(left, level);
    level->balance -= MIN(left->balance, 0) - 1;
    left->balance += MAX(level->balance, 0) + 1;
    if (tree->root == level)
        tree->root = left;
}

static inline void
LevelBucket_Unlink(struct Level *level) {
    struct Level *parent = level->parent;
    if (parent != NULL)
        (parent->left == level) ? LevelBucket_SetLeft(parent, NULL)
                                : LevelBucket_SetRight(parent, NULL);
    Level_Delete(level);
}

#include <stdio.h>
static inline void
LevelBucket_Swap(struct LevelBucket *tree, struct Level *level, struct Level *successor) {
    if (level->left == successor)
        LevelBucket_RotateRight(tree, level);
    else if (level->right == successor)
        LevelBucket_RotateLeft(tree, level);
    else {
        struct Level *tempLevel = level->parent;
        LevelBucket_SetParent(level, successor->parent);
        LevelBucket_SetParent(successor, tempLevel);

        tempLevel = level->right;
        LevelBucket_SetRight(level, successor->right);
        LevelBucket_SetRight(successor, tempLevel);

        tempLevel = level->left;
        LevelBucket_SetLeft(level, successor->left);
        LevelBucket_SetLeft(successor, tempLevel);

        i8 tempBalance = level->balance;
        level->balance = successor->balance;
        successor->balance = tempBalance;
    }
}

static inline void
LevelBucket_DestructRecursive(struct Level *level) {
    if (level == NULL)
        return;
    LevelBucket_DestructRecursive(level->left);
    LevelBucket_DestructRecursive(level->right);
    Level_Delete(level);
}

static inline void
LevelBucket_AddFixUp(struct LevelBucket *tree, struct Level *level) {
    for (struct Level *current = level, *parent = current->parent;
         parent != NULL;
         current = parent, parent = current->parent)
    {
        if (parent->left == current) {
            switch (--(parent->balance)) {
                case 0:
                    return;
                case 1:
                    continue;
                case -2:
                    if (current->balance == 1)
                        LevelBucket_RotateLeft(tree, current);
                    LevelBucket_RotateRight(tree, parent);
                    return;
            }
        } else {
            switch (++(parent->balance)) {
                case 0:
                    return;
                case 1:
                    continue;
                case 2:
                    if (current->balance == -1)
                        LevelBucket_RotateRight(tree, current);
                    LevelBucket_RotateLeft(tree, parent);
                    return;
            }
        }
    }
}

static inline void
LevelBucket_RemoveFixUp(struct LevelBucket *tree, struct Level *level) {
    for (struct Level *current = level, *parent = level->parent, *sibling;
         parent != NULL;
         current = parent, parent = current->parent)
    {
        if (parent->left == current) {
            switch (++(parent->balance)) {
                case 0:
                    continue;
                case 1:
                    return;
                case 2:
                    sibling = parent->right;
                    if (sibling->balance == -1)
                        LevelBucket_RotateRight(tree, sibling);
                    LevelBucket_RotateLeft(tree, parent);
                    parent = parent->parent;
                    if (parent->balance == -1)
                        return;
                    continue;
            }
        } else {
            switch (--(parent->balance)) {
                case 0:
                    continue;
                case -1:
                    return;
                case -2:
                    sibling = parent->left;
                    if (sibling->balance == 1)
                        LevelBucket_RotateLeft(tree, sibling);
                    LevelBucket_RotateRight(tree, parent);
                    parent = parent->parent;
                    if (parent->balance == 1)
                        return;
                    continue;
            }
        }
    }
}

void
LevelBucket_Construct(struct LevelBucket *tree) {
    tree->root = NULL;
    tree->best = NULL;
}

void
LevelBucket_Destruct(struct LevelBucket *tree) {
    LevelBucket_DestructRecursive(tree->root);
}

struct Level *
LevelBucket_GetOrAdd(struct LevelBucket *tree, i32 price) {
    if (tree->root == NULL)
        return tree->root = tree->best = Level_New(price);
    if (price == tree->best->price)
        return tree->best;
    if (price > tree->best->price) {
        struct Level *best = Level_New(price);
        LevelBucket_SetRight(tree->best, best);
        LevelBucket_AddFixUp(tree, best);
        return tree->best = best;
    }
    struct Level *current = tree->root;
    for (;;) {
        if (price == current->price)
            return current;
        if (price < current->price) {
            if (current->left == NULL)
                break;
            current = current->left;
        } else {
            if (current->right == NULL)
                break;
            current = current->right;
        }
    }
    struct Level *level = Level_New(price);
    LevelBucket_SetParent(level, current);
    LevelBucket_AddFixUp(tree, level);
    return level;
}

void
LevelBucket_Remove(struct LevelBucket *tree, struct Level *level) {
    if (tree->best == level) {
        if (level->parent == level->left) {
            tree->root = tree->best = NULL;
            LevelBucket_Unlink(level);
            return;
        }
        if (level->left != NULL)
            LevelBucket_Swap(tree, level, level->left);
        tree->best = level->parent;
        LevelBucket_RemoveFixUp(tree, level);
        LevelBucket_Unlink(level);
        return;
    }
    for (;;) {
        if (level->left == level->right) {
            LevelBucket_RemoveFixUp(tree, level);
            LevelBucket_Unlink(level);
            return;
        } else {
            struct Level *successor = (level->left != NULL) ? level->left : level->right;
            while (successor->right != NULL)
                successor = successor->right;
            LevelBucket_Swap(tree, level, successor);
        }
    }
}
