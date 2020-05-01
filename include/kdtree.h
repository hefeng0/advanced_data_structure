#include <stdbool.h>
#include <stdlib.h>
#include "list.h"

typedef struct kdnode {
	double * value;
	double * min;
	double * max;
	struct kdnode * left;
	struct kdnode * right;
} kdnode;

typedef struct kdtree {
	kdnode * root;
	int dim;
} kdtree;

kdtree * kdtree_init(int dim);

bool kdtree_node_init(kdtree *tree, kdnode *node);

void kdtree_node_destroy(kdnode *node);

void kdtree_destroy(kdtree *tree);

int kdtree_insert(kdtree *tree, kdnode *node);

void kdtree_del(kdtree *tree, kdnode *node);

kdnode * kdtree_nearest(kdtree *tree, double *vals);

void kdtree_nearest_range(kdtree *tree, double *vals, double range, list *l);

void kdtree_print(kdtree *tree);
