/// @file main.c
/// @Synopsis
/// @author hnu_hefeng@qq.com
/// @version 0.0.1
/// @date 2020-01-11
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "itree.h"

typedef struct test_node {
	int value;
} test_node;

int main(int argc, char **argv)
{
	itree * tree = itree_init();
	int lows[] = {1, 4, 2, 3, 9, 7, 8};
	int highs[] = {5, 5, 6, 4, 10, 9, 9};
	int count = sizeof(lows)/sizeof(int);
	inode * inodes = malloc(count*sizeof(inode));
	for ( int i=0;i<count;i++ ) {
		itree_node_init(inodes+i, lows[i], highs[i]);
		itree_insert(tree, inodes+i);
	}

	interval i = {
		.low = 2,
		.high = 9,
		.max = 3,
	};

	itree_print(tree);

	list * l = malloc(sizeof(list));
	list_init(l);

	itree_find(tree, &i, l);
	void * value = NULL;
	printf("itree_find interval[low:%d, high:%d] result:\n", i.low, i.high);
	while((value=list_del(l)) != VALUE_INVALID ) {
		interval *ivalue = (interval*)value;
		printf("low=%d high=%d\n", ivalue->low, ivalue->high);
	}

	free(l);
	free(inodes);
	itree_destroy(tree);

  	return 0;
}
