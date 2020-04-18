/// @file main.c
/// @Synopsis
/// @author hnu_hefeng@qq.com
/// @version 0.0.1
/// @date 2020-01-11
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dlist.h"

typedef struct test_node {
	int data;
	struct dlist_head list;
} test_node;

int main(int argc, char **argv)
{
	struct dlist_head head;
	dlist_init(&head);
	test_node node1 = {
		.data = 1,
		.list = INIT_LIST_HEAD(head),
	};
	test_node node2 = {
		.data = 2,
		.list = INIT_LIST_HEAD(head),
	};
	test_node node3 = {
		.data = 3,
		.list = INIT_LIST_HEAD(head),
	};
	dlist_add(&node1.list, &head);
	dlist_add(&node2.list, &head);
	dlist_add(&node3.list, &head);

	test_node *pos;
	container_for_each(pos, &head, list) {
		printf("data=%d\n", pos->data);
	}

  	return 0;
}
