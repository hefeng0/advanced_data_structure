/// @file main.c
/// @Synopsis
/// @author hnu_hefeng@qq.com
/// @version 0.0.1
/// @date 2020-01-11
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rbtree.h"

typedef struct test_node {
	int value;
	rbnode node;
} test_node;

int main(int argc, char **argv)
{
	test_node test_nodes[10];
	rbtree * tree = rbtree_init(NULL);
	for ( int i=9;i>=0;i-- ) {
		rbtree_node_init(&test_nodes[i].node, i);
		test_nodes[i].value = i+100;
		rbtree_insert(tree, &test_nodes[i].node);
	}

	rbtree_print(tree);
	rbtree_check(tree);

	int key=3;
	rbnode *node = rbtree_find(tree, key);
	test_node * tnode = container_of(node, test_node, node);
	printf("key=%d tnode->value=%d\n", key, tnode->value);

	int del=7;
	printf("after delete %d\n", test_nodes[del].node.key);
	rbtree_del(tree, &test_nodes[del].node);
	rbtree_print(tree);

	rbtree_check(tree);

	key=3;
	node = rbtree_find(tree, key);
	tnode = container_of(node, test_node, node);
	printf("key=%d tnode->value=%d\n", key, tnode->value);

	rbtree_destroy(tree);
	
  	return 0;
}
