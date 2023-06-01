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

#include <stdio.h>
static inline void
LevelTree_Swap(struct LevelTree *tree, struct Level *level, struct Level *successor) {
    if (level->left == successor)
        LevelTree_RotateRight(tree, level);
    else if (level->right == successor)
        LevelTree_RotateLeft(tree, level);
    else {
        struct Level *tempLevel = level->parent;
        LevelTree_SetParent(level, successor->parent);
        LevelTree_SetParent(successor, tempLevel);

        tempLevel = level->right;
        LevelTree_SetRight(level, successor->right);
        LevelTree_SetRight(successor, tempLevel);

        tempLevel = level->left;
        LevelTree_SetLeft(level, successor->left);
        LevelTree_SetLeft(successor, tempLevel);

        i8 tempBalance = level->balance;
        level->balance = successor->balance;
        successor->balance = tempBalance;
    }
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
                        LevelTree_RotateLeft(tree, current);
                    LevelTree_RotateRight(tree, parent);
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
                        LevelTree_RotateRight(tree, current);
                    LevelTree_RotateLeft(tree, parent);
                    return;
            }
        }
    }
}

void
print(struct Level *level, int depth) {
    if (level == NULL)
        return;
    print(level->left, depth + 1);
    for (int i = 0 ; i < depth; i++) {
        printf("\t");
    }
    printf("%d(%d)\n", level->price, level->balance);
    print(level->right, depth + 1);
}

static inline void
LevelTree_RemoveFixUp(struct LevelTree *tree, struct Level *level) {
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
                        LevelTree_RotateRight(tree, sibling);
                    LevelTree_RotateLeft(tree, parent);
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
                        LevelTree_RotateLeft(tree, sibling);
                    LevelTree_RotateRight(tree, parent);
                    parent = parent->parent;
                    if (parent->balance == 1)
                        return;
                    continue;
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

struct Level *
LevelTree_GetOrAdd(struct LevelTree *tree, i32 price) {
    if (tree->root == NULL)
        return tree->root = tree->best = Level_New(price);
    if (price == tree->best->price)
        return tree->best;
    if (price > tree->best->price) {
        struct Level *best = Level_New(price);
        LevelTree_SetRight(tree->best, best);
        LevelTree_AddFixUp(tree, best);
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
    LevelTree_SetParent(level, current);
    LevelTree_AddFixUp(tree, level);
    return level;
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
