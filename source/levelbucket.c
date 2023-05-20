#include "level.h"
#include "levelbucket.h"
#include "levelheap.h"
#include "pool.h"

static inline void setLeft(struct Level *level, struct Level *left) {
    if (level != NULL)
        level->left = left;
    if (left != NULL)
        left->parent = level;
} /* setLeft */

static inline void setRight(struct Level *level, struct Level *right) {
    if (level != NULL)
        level->right = right;
    if (right != NULL)
        right->parent = level;
} /* setRight */

static inline i8 updateBalance(struct Level *level) {
    i8 leftBalance = (level->left != NULL) ? level->left->balance + 1 : 0;
    i8 rightBalance = (level->right != NULL) ? level->right->balance + 1 : 0;
    level->balance = rightBalance - leftBalance;
    return level->balance;
} /* updateBalance */

static inline struct Level *rotateLeft(struct Level *level) {
    struct Level *parent = level->parent, *right = level->right;
    (parent != NULL && parent->left == level) ? setLeft(parent, right)
                                              : setRight(parent, right);
    setRight(level, right->left);
    setLeft(right, level);
    updateBalance(level);
    updateBalance(right);
    return right;
} /* rotateLeft */

static inline struct Level *rotateRight(struct Level *level) {
    struct Level *parent = level->parent, *left = level->left;
    (parent != NULL && parent->left == level) ? setLeft(parent, left)
                                              : setRight(parent, left);
    setLeft(level, left->right);
    setRight(left, level);
    updateBalance(level);
    updateBalance(left);
    return left;
} /* rotateRight */

static inline void fixUp(struct Level *level) {
    while (level != NULL) {
        if (level->balance == updateBalance(level))
            return;
        if (level->balance == -2) {
            if (level->left->balance == 1)
                rotateLeft(level->left);
            level = rotateRight(level);
        } else if (level->balance == 2) {
            if (level->right->balance == -1)
                rotateRight(level->right);
            level = rotateLeft(level);
        } /* if */
        level = level->parent;
    } /* while */
} /* fixUp */

static inline void swap(struct Level *level, struct Level *successor) {
    if (level->left == successor)
        rotateRight(level);
    else if (level->right == successor)
        rotateLeft(level);
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
    } /* if */
} /* swap */

struct Level *getOrAddRecursive(struct Level *level, struct LevelHeap *heap, i32 price) {
    if (level == NULL) {
        level = (struct Level *) Pool_alloc(heap->pool);
        Level_init(level, heap, price);
        return level;
    } /* if */
    if (price < level->price) {
        struct Level *left = getOrAddRecursive(level->left, heap, price);
        if (level->left == NULL) {
            setLeft(level, left);
            fixUp(level);
        } /* if */
        return left;
    } /* if */
    if (price > level->price) {
        struct Level *right = getOrAddRecursive(level->right, heap, price);
        if (level->right == NULL) {
            setRight(level, right);
            fixUp(level);
        } /* if */
        return right;
    } /* if */
    return level;
} /* getOrAddRecursive */

static struct Level *removeRecursive(struct Level *level) {
    if (level->left == level->right) {
        struct Level *parent = level->parent;
        (parent != NULL && parent->left == level) ? setLeft(parent, NULL)
                                                  : setRight(parent, NULL);
        Pool_free(level->heap->pool, (void *) level);
        fixUp(parent);
        return parent;
    } else {
        struct Level *successor = level->left;
        if (successor == NULL)
            successor = level->right;
        else if (level->right != NULL) {
            while (successor->right != NULL)
                successor = successor->right;
        } /* if */
        swap(level, successor);
        removeRecursive(level);
        return successor;
    } /* if */
} /* removeRecursive */

void LevelBucket_init(struct LevelBucket *bucket) {
    bucket->root = NULL;
    bucket->best = NULL;
} /* LevelBucket_init */

struct Level *LevelBucket_getOrAdd(struct LevelBucket *bucket, struct LevelHeap *heap, i32 price) {
    struct Level *root = (bucket->best != NULL && price >= bucket->best->price) ? bucket->best
                                                                                : bucket->root;
    struct Level *level = getOrAddRecursive(root, heap, price);
    if (bucket->root == NULL)
        bucket->root = bucket->best = level;
    else {
        if (bucket->root->parent != NULL)
            bucket->root = bucket->root->parent;
        if (bucket->best->right != NULL)
            bucket->best = bucket->best->right;
    } /* if */
    return level;
} /* LevelBucket_getOrAdd */

void LevelBucket_remove(struct LevelBucket *bucket, struct Level *level) {
    struct Level *successor = removeRecursive(level);
    if (level == bucket->root)
        bucket->root = successor;
    if (level == bucket->best)
        bucket->best = successor;
    if (bucket->root != NULL && bucket->root->parent != NULL)
        bucket->root = bucket->root->parent;
} /* LevelBucket_remove */
