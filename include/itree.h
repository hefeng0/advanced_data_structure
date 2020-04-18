#include <stdbool.h>
#include <stdlib.h>
#include "rbtree.h"
#include "list.h"

typedef struct interval {
	int low;
	int high;
	int max;
} interval;

//interval node
typedef struct inode {
	interval i;
	rbnode node;
} inode;

typedef struct itree {
	rbtree * root;
} itree;

itree * itree_init();

void itree_node_init(inode *node, int low, int high);

void itree_destroy(itree *tree);

int itree_insert(itree *tree, inode *node);

void itree_del(itree *tree, inode *node);

void itree_find(itree *tree, interval *i, list *l);

void itree_print(itree *tree);
