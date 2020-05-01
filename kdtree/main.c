/// @file main.c
/// @Synopsis
/// @author hnu_hefeng@qq.com
/// @version 0.0.1
/// @date 2020-01-11
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kdtree.h"

typedef struct test_node {
	kdnode node;
	int value;
} test_node;

int main(int argc, char **argv)
{
	int dim = 2;
	int sample_num = 10;
	kdtree * tree = kdtree_init(dim);
	test_node * nodes = malloc(sizeof(test_node) * sample_num);
	for ( int i=0;i<sample_num;i++ ) {
		kdtree_node_init(tree, &nodes[i].node);
		for ( int j=0;j<dim;j++ ) {
			nodes[i].node.value[j] = rand() % 20;
		}
		nodes[i].value = i;
		kdtree_insert(tree, &nodes[i].node);
	}

	kdtree_print(tree);

	//测试查找最近邻节点
	double *test_value = malloc(sizeof(double)*dim);
	for ( int i=0;i<dim;i++ )
		test_value[i] = rand() % 20;

	kdnode * result = kdtree_nearest(tree, test_value);
	printf("\n\nfind_nearest node=[");
	for ( int i=0;i<dim;i++ ) {
		printf("%lf ", test_value[i]);
	}
	test_node * node = container_of(result, test_node, node);
	printf("] is [value=%d ", node->value);
	for ( int i=0;i<dim;i++ ) {
		printf("%lf ", result->value[i]);
	}
	printf("]\n\n\n");

	list l;
	list_init(&l);
	kdtree_nearest_range(tree, test_value, 8, &l);
	printf("find_nearest_range node=[");
	for ( int i=0;i<dim;i++ ) {
		printf("%lf ", test_value[i]);
	}
	printf("] is:\n");
	int i = 0;
	void *value = NULL;
	while((value=list_del(&l)) != VALUE_INVALID) {
		kdnode * node = (kdnode*)value;
		i++;
		printf("%d:[", i);
		for ( int j=0;j<dim;j++ ) {
			printf("%lf ", node->value[j]);
		}
		printf("]\n");
	}
	list_destroy(&l);

	for ( int i=0;i<sample_num;i++ ) {
		kdtree_node_destroy(&nodes[i].node);
	}
	free(nodes);
	kdtree_destroy(tree);

  	return 0;
}
