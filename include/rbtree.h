#include <stdbool.h>
#include <stdlib.h>
#include "dlist.h"

enum RBNODE_COLOR {
	COLOR_RED,
	COLOR_BLACK,
};

typedef struct rbnode {
	int key;
	int color;
	struct rbnode * parent;
	struct rbnode * left;
	struct rbnode * right;
} rbnode;

typedef struct rbtree {
	rbnode * root;
	int count;
} rbtree;

rbtree * rbtree_init();

void rbtree_node_init(rbnode *Node, int key);

void rbtree_destroy(rbtree *tree);

int rbtree_insert(rbtree *tree, rbnode *node);

void rbtree_del(rbtree *tree, rbnode *node);

rbnode * rbtree_find(rbtree *tree, int key);

void rbtree_print(rbtree *tree);

void rbtree_check(rbtree *tree);
