#include <stdbool.h>
#include <stdlib.h>
#include "dlist.h"

#define MAX_LEVEL 32

struct skiplist {
	int count;
	struct list_head list[MAX_LEVEL];
};

typedef struct sl_node {
	int key;
	int level;
	//struct list_head list[0];
	struct list_head list[MAX_LEVEL];
} sl_node;

struct skiplist * skiplist_init();

void skiplist_node_init(sl_node *node, int key);

void skiplist_insert(struct skiplist *sl, sl_node *node);

sl_node *skiplist_find(struct skiplist *sl, int key);

void skiplist_del(struct skiplist *sl, int key);

void skiplist_print(struct skiplist *sl);

void skiplist_destroy(struct skiplist *sl);
