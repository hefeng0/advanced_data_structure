
#include <stdio.h>
#include "itree.h"

void itree_node_ext_fixup(rbnode *node)
{
	if ( rbtree_is_leaf(node) )
		return;
	inode * interval_node = container_of(node, inode, node);
	interval_node->i.max = interval_node->i.high;
	if ( !rbtree_is_leaf(node->left) ) {
		inode * left_inode = container_of(node->left, inode, node);
		if ( left_inode->i.max > interval_node->i.max )
			interval_node->i.max = left_inode->i.max;
	}
	if ( !rbtree_is_leaf(node->right) ) {
		inode * right_inode = container_of(node->right, inode, node);
		if ( right_inode->i.max > interval_node->i.max )
			interval_node->i.max = right_inode->i.max;
	}
}

itree * itree_init()
{
	itree * tree = malloc(sizeof(itree));
	tree->root = rbtree_init(itree_node_ext_fixup);
	return tree;
}

void itree_node_init(inode *node, int low, int high)
{
	rbtree_node_init(&node->node, low);
	node->i.low = low;
	node->i.high = high;
	node->i.max = high;
}

void itree_destroy(itree *tree)
{
	if ( !tree )
		return;
	if ( tree->root )
		rbtree_destroy(tree->root);
	free(tree);
}

int itree_insert(itree *tree, inode *node)
{
	return rbtree_insert(tree->root, &node->node);
}

void itree_del(itree *tree, inode *node)
{
	return rbtree_del(tree->root, &node->node);
}

bool interval_overlap(interval *l, interval *r)
{
	return (l->low < r->high) && (r->low < l->high );
}

void find_interval(rbnode *node, interval *i, list *l)
{
	if ( rbtree_is_leaf(node) )
		return;
	struct inode * inode = container_of(node, struct inode, node);
	if ( interval_overlap(&inode->i, i) )
		list_add(l, &inode->i);

	if ( !rbtree_is_leaf(node->left) ) {
		struct inode * inode = container_of(node->left, struct inode, node);
		if ( i->low < inode->i.max )
			find_interval(node->left, i, l);
	}

	if ( !rbtree_is_leaf(node->right) ) {
		struct inode * inode = container_of(node->right, struct inode, node);
		if ( i->high > inode->i.low )
			find_interval(node->right, i, l);
	}
}

void itree_find(itree *tree, interval *i, list *l)
{
	return find_interval(tree->root->root, i, l);
}

void print_node(rbnode *node, int left_or_right) {
	if ( rbtree_is_leaf(node) )
		return;
	
	inode * interval_node = container_of(node, inode, node);
	printf("%s %d(%c,[low:%d,high:%d,max:%d])%d\t", left_or_right==1?"left":"right", \
			node->key, COLOR(node)==COLOR_RED?'r':'b', \
			interval_node->i.low, interval_node->i.high, interval_node->i.max, \
			node->parent?node->parent->key:-1);
	print_node(node->left, 1);
	print_node(node->right, 2);
	printf("\n");
}

void itree_print(itree *tree)
{
	print_node(tree->root->root, 0);
}
