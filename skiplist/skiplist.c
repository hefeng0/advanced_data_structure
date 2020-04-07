#include <time.h>
#include <stdio.h>
#include "skiplist.h"

struct skiplist * skiplist_init()
{
	struct skiplist *sl = NULL;
	sl = malloc(sizeof(struct skiplist));
	if (!sl)
		return NULL;

	for ( int i=0;i<MAX_LEVEL;i++ ) {
		list_init(&sl->list[i]);
	}
	sl->count = 0;

	return sl;
}

void skiplist_node_init(sl_node *node, int key)
{
	int level = random() % MAX_LEVEL;
	node->key = key;
	node->level = level;
}

void node_del(sl_node *node)
{
	if ( !node )
		return;
	for ( int i=node->level;i>=0;i-- ) {
		list_del(&node->list[i]);
	}
}

void skiplist_insert(struct skiplist *sl, sl_node *node)
{
	int key = node->key;
	struct list_head * prev = &sl->list[MAX_LEVEL-1];
	struct list_head * cur = prev->next;
	for ( int i = MAX_LEVEL-1;i>=0;i-- ) {
		list_for_each(cur, &sl->list[i]) {
			sl_node *node = container_of(cur, sl_node, list[i]);
			if ( key < node->key )
				break;
			prev = cur;
		}
		if ( i <= node->level ) {
			struct list_head list = INIT_LIST_HEAD(sl->list[i]);
			node->list[i] = list;
			list_add(&node->list[i], prev);
		}
		prev--;
		//现在prev还是sl->list[i]
		if ( i>node->level )
			cur = prev->next;
		else {
			//插入到了本层的头节点
			if ( (prev+1) == &sl->list[i] )
				cur = prev->next;
			else
				cur = prev;
		}
	}
	sl->count++;

}

sl_node * skiplist_find(struct skiplist *sl, int key)
{
	bool found_level = false;
	struct list_head * prev = &sl->list[MAX_LEVEL-1];
	for ( int i=MAX_LEVEL-1;i>=0;i-- ) {
		struct list_head * cur = found_level ? prev:prev->next;
		list_for_each(cur, &sl->list[i]) {
			sl_node * node = container_of(cur, sl_node, list[i]);
			if ( key == node->key )
				return node;
			if ( key < node->key )
				break;
			found_level = true;
			prev = cur;
		}
		prev--;
	}

	return NULL;
}

void skiplist_del(struct skiplist *sl, int key)
{
	sl_node * node = skiplist_find(sl, key);
	if ( !node )
		return;

	node_del(node);
	sl->count--;
}

void skiplist_destroy(struct skiplist *sl)
{
	free(sl);
}

void skiplist_print(struct skiplist *sl)
{
	printf("----------------------------\n");
	for ( int i=MAX_LEVEL-1;i>=0;i-- ) {
		struct list_head * cur = sl->list[i].next;
		printf("level=%d\n", i);
		list_for_each(cur, &sl->list[i]) {
			sl_node *node = container_of(cur, sl_node, list[i]);
			printf("key=%d\t ", node->key);
		}
		printf("\n");
	}
	printf("total count:%d\n", sl->count);
}
