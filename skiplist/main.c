/// @file main.c
/// @Synopsis
/// @author hnu_hefeng@qq.com
/// @version 0.0.1
/// @date 2020-01-11
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "skiplist.h"

typedef struct test_node {
	sl_node node;
	int64_t value;
}test_node;

void test() {
	struct skiplist *sl = skiplist_init();
	test_node test_nodes[5];
	for ( int i=0;i<5;i++ ) {
		int key = i;
		skiplist_node_init(&test_nodes[i].node, key);
		test_nodes[i].value = key+100;
		skiplist_insert(sl, &test_nodes[i].node);
	}

	skiplist_print(sl);

	sl_node * node = skiplist_find(sl, 3);
	test_node *tnode = container_of(node, test_node, node);
	printf("key=%d value=%ld\n", node->key, tnode->value);
	
	skiplist_del(sl, 3);
	skiplist_print(sl);

	skiplist_destroy(sl);
}

int main(int argc, char **argv)
{
	test();
	
  	return 0;
}
