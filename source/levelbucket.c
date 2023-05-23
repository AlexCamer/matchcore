#include "level.h"
#include "levelbucket.h"
#include "levelheap.h"
#include "pool.h"

static inline void
LevelBucket_setLeft(struct Level *level, struct Level *left) {
    if (level != NULL)
        level->left = left;
    if (left != NULL)
        left->parent = level;
}

static inline void
LevelBucket_setRight(struct Level *level, struct Level *right) {
    if (level != NULL)
        level->right = right;
    if (right != NULL)
        right->parent = level;
}

static inline i8
LevelBucket_updateBalance(struct Level *level) {
    i8 leftBalance = (level->left != NULL) ? level->left->balance + 1 : 0;
    i8 rightBalance = (level->right != NULL) ? level->right->balance + 1 : 0;
    level->balance = rightBalance - leftBalance;
    return level->balance;
}

static inline struct Level *
LevelBucket_rotateLeft(struct Level *level) {
    struct Level *parent = level->parent, *right = level->right;
    (parent != NULL && parent->left == level) ? LevelBucket_setLeft(parent, right)
                                              : LevelBucket_setRight(parent, right);
    LevelBucket_setRight(level, right->left);
    LevelBucket_setLeft(right, level);
    LevelBucket_updateBalance(level);
    LevelBucket_updateBalance(right);
    return right;
}

static inline struct Level *
LevelBucket_rotateRight(struct Level *level) {
    struct Level *parent = level->parent, *left = level->left;
    (parent != NULL && parent->left == level) ? LevelBucket_setLeft(parent, left)
                                              : LevelBucket_setRight(parent, left);
    LevelBucket_setLeft(level, left->right);
    LevelBucket_setRight(left, level);
    LevelBucket_updateBalance(level);
    LevelBucket_updateBalance(left);
    return left;
}

static inline void
LevelBucket_fixUp(struct Level *level) {
    while (level != NULL) {
        if (level->balance == LevelBucket_updateBalance(level))
            return;
        if (level->balance == -2) {
            if (level->left->balance == 1)
                LevelBucket_rotateLeft(level->left);
            level = LevelBucket_rotateRight(level);
        } else if (level->balance == 2) {
            if (level->right->balance == -1)
                LevelBucket_rotateRight(level->right);
            level = LevelBucket_rotateLeft(level);
        }
        level = level->parent;
    }
}

static inline void
LevelBucket_swap(struct Level *level, struct Level *successor) {
    if (level->left == successor)
        LevelBucket_rotateRight(level);
    else if (level->right == successor)
        LevelBucket_rotateLeft(level);
    else {
        struct Level *tempLevel = level->parent;
        level->parent = successor->parent;
        successor->parent = tempLevel;

        tempLevel = level->left;
        level->left = successor->left;
        successor->left = tempLevel;

        tempLevel = level->right;
        level->right = successor->right;
        successor->right = tempLevel;

        i8 tempBalance = level->balance;
        level->balance = successor->balance;
        successor->balance = tempBalance;
    }
}

static inline void
LevelBucket_destructRecursive(struct Level *level) {
    if (level == NULL)
        return;
    LevelBucket_destructRecursive(level->left);
    LevelBucket_destructRecursive(level->right);
    Level_delete(level);
}

static inline struct Level *
LevelBucket_getOrAddRecursive(struct Level *level, i32 price) {
    if (price < level->price) {
        if (level->left == NULL)
            LevelBucket_setLeft(level, Level_new(price));
        return LevelBucket_getOrAddRecursive(level->left, price);
    }
    if (price > level->price) {
        if (level->right == NULL) {
            LevelBucket_setRight(level, Level_new(price));
        return LevelBucket_getOrAddRecursive(level->right, price);
    }
    LevelBucket_fixUp(level);
    return level;
}

static inline struct Level *
LevelBucket_removeRecursive(struct Level *level) {
    struct Level *successor;
    if (level->left == level->right) {
        successor = level->parent;
        (successor != NULL && successor->left == level) ? LevelBucket_setLeft(successor, NULL)
                                                        : LevelBucket_setRight(successor, NULL);
        Level_delete(level);
        LevelBucket_fixUp(successor);
    } else {
        successor = (level->left != NULL) ? level->left : level->right;
        while (successor->right != NULL)
            successor = successor->right;
        LevelBucket_swap(level, successor);
        LevelBucket_removeRecursive(level);
    }
    return successor;
}

void
LevelBucket_construct(struct LevelBucket *bucket) {
    bucket->root = NULL;
    bucket->best = NULL;
}

void LevelBucket_destruct(struct LevelBucket *bucket) {
    LevelBucket_destructRecursive(bucket->root);
}

struct Level *
LevelBucket_getOrAdd(struct LevelBucket *bucket, i32 price) {
    struct Level *level;
    if (bucket->root == NULL)
        level = bucket->root = bucket->best = Level_new(price);
    else {
        level = (price >= bucket->best->price) ? bucket->best : bucket->root;
        level = LevelBucket_getOrAddRecursive(level, price);
        if (bucket->root->parent != NULL)
            bucket->root = bucket->root->parent;
        if (bucket->best->right != NULL)
            bucket->best = bucket->best->right;
    }
    return level;
}

void
LevelBucket_remove(struct LevelBucket *bucket, struct Level *level) {
    struct Level *successor = LevelBucket_removeRecursive(level);
    if (level == bucket->root)
        bucket->root = successor;
    if (level == bucket->best)
        bucket->best = successor;
    if (bucket->root != NULL && bucket->root->parent != NULL)
        bucket->root = bucket->root->parent;
}
