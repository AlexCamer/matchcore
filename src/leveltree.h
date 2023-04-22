#ifndef __LEVEL_TREE_H__
#define __LEVEL_TREE_H__

#define RED   0
#define BLACK 1

struct Book;

struct LevelNode {
    struct Book *book;
    struct LevelNode *parent;
    struct LevelNode *left;
    struct LevelNode *right;
    struct Level level;
    u8 color:1;
    u8 cached:1;
};

struct LevelTree {
    struct LevelNode *root;
    struct LevelNode *best;
    struct Level cache;
};

#endif /* __LEVEL_TREE_H__ */
