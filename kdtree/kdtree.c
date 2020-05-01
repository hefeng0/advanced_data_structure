#include <stdio.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "kdtree.h"

kdtree * kdtree_init(int dim)
{
	kdtree * tree = NULL;
	tree = malloc(sizeof(kdtree));
	if ( !tree )
		return NULL;
	tree->root = NULL;
	tree->dim = dim;
	return tree;
}

bool kdtree_node_init(kdtree *tree, kdnode *node)
{
	node->value = NULL;
	node->value = malloc(sizeof(double) * tree->dim);
	node->min = malloc(sizeof(double) * tree->dim);
	node->max = malloc(sizeof(double) * tree->dim);
	if ( !node->value || !node->min || !node->max )
		return false;

	node->left = NULL;
	node->right = NULL;

	return true;
}

void kdtree_node_destroy(kdnode *node) {
	if ( !node )
		return;
	if ( node->value )
		free(node->value);
	if ( node->min )
		free(node->min);
	if ( node->max )
		free(node->max);
}

void kdtree_destroy(kdtree *tree)
{
	if ( !tree )
		return;
	free(tree);
}

int kdtree_insert(kdtree *tree, kdnode *node)
{
	memcpy(node->min, node->value, sizeof(double)*tree->dim);
	memcpy(node->max, node->value, sizeof(double)*tree->dim);
	int idx = 0;
	kdnode ** pos = &tree->root;
	while(*pos) {
		for ( int i=0;i<tree->dim;i++ ) {
			if ( node->value[i]<(*pos)->min[i] )
				(*pos)->min[i] = node->value[i];
			if ( node->value[i]>(*pos)->max[i] )
				(*pos)->max[i] = node->value[i];
		}
		double val = (*pos)->value[idx];
		if ( node->value[idx] < val )
			pos = &(*pos)->left;
		else
			pos = &(*pos)->right;
		idx = (idx+1) % tree->dim;
	}

	*pos = node;
	return 0;
}

double two_point_dist(int dim, double *left, double *right)
{
	double dist = 0;
	for ( int i=0;i<dim;i++ ) {
		dist += (left[i]-right[i]) * (left[i]-right[i]);
	}
	return dist;
}

double point_to_hyperrect_min_dist(kdnode *node, int dim, double *vals)
{
	double result = 0;
	for ( int i=0;i<dim;i++ ) {
		if ( vals[i] < node->min[i] )
			result += (vals[i]-node->min[i]) * (vals[i]-node->min[i]);
		else if ( vals[i] > node->max[i] )
			result += (vals[i]-node->max[i]) * (vals[i]-node->max[i]);
	}
	return result;
}

void __kdtree_nearest(kdtree *tree, int idx, kdnode *node, double *vals, kdnode **target, double *min_dist)
{
	if ( !node )
		return;

	double node_dist = two_point_dist(tree->dim, node->value, vals);
	if ( node_dist < *min_dist ) {
		*target = node;
		*min_dist = node_dist;
	}
	if ( vals[idx] < node->value[idx] ) {
		idx = (idx+1) % tree->dim;
		__kdtree_nearest(tree, idx, node->left, vals, target, min_dist);
		if ( node->right ) {
			double right_min_dist = point_to_hyperrect_min_dist(node->right, tree->dim, vals);
			if ( right_min_dist < *min_dist )
				__kdtree_nearest(tree, idx, node->right, vals, target, min_dist);
		}
	} else {
		idx = (idx+1) % tree->dim;
		__kdtree_nearest(tree, idx, node->right, vals, target, min_dist);
		if ( node->left ) {
			double left_min_dist = point_to_hyperrect_min_dist(node->left, tree->dim, vals);
			if ( left_min_dist < *min_dist )
				__kdtree_nearest(tree, idx, node->left, vals, target, min_dist);
		}
	}
}

kdnode * kdtree_nearest(kdtree *tree, double *vals)
{
	kdnode * target = NULL;
	double min_dist = DBL_MAX;
	__kdtree_nearest(tree, 0, tree->root, vals, &target, &min_dist);
	return target;
}

void __kdtree_nearest_range(kdtree *tree, int idx, kdnode *node, double *vals, double range, list *l)
{
	if ( !node )
		return;
	double node_dist = two_point_dist(tree->dim, node->value, vals);
	if ( sqrt(node_dist) <= range ) {
		list_add(l, node);
	}
	double dx = vals[idx] - node->value[idx];
	idx = (idx + 1) % tree->dim;
	__kdtree_nearest_range(tree, idx, dx <= 0.0 ? node->left:node->right, vals, range, l);
	//其中任意一侧idx维距离已经超过range，另一侧就不用找了
	if ( fabs(dx) < range ) {
		__kdtree_nearest_range(tree, idx, dx <= 0.0 ? node->right:node->left, vals, range, l);
	}
}

void kdtree_nearest_range(kdtree *tree, double *vals, double range, list *l)
{
	return __kdtree_nearest_range(tree, 0, tree->root, vals, range, l);
}

void print_kdnode(kdnode *node, int dim, int left_or_right)
{
	if ( !node )
		return;
	
	char pos[10] = "";
	if ( left_or_right != 0 )
		printf("%s[", left_or_right == 1?"left":"right"); 
	else
		printf("[");
	for ( int i=0;i<dim;i++ ) {
		printf("%lf ", node->value[i]);
	}
	printf("]min[");
	for ( int i=0;i<dim;i++ ) {
		printf("%lf ", node->min[i]);
	}
	printf("]max[");
	for ( int i=0;i<dim;i++ ) {
		printf("%lf ", node->max[i]);
	}
	printf("]\n");
	
	print_kdnode(node->left, dim, 1);
	print_kdnode(node->right, dim, 2);
}

void kdtree_print(kdtree *tree)
{
	print_kdnode(tree->root, tree->dim, 0);
}
