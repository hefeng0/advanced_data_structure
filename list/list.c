#include "list.h"

typedef struct lnode {
	struct dlist_head list;
	void * value;
} lnode;

void list_init(struct list *head)
{
	dlist_init(&head->head);
}

/*
 * 将value插入到头部
 */
void list_add(struct list *head, void *value)
{
	lnode * node = NULL;
	node = malloc(sizeof(lnode));
	if ( !node )
		return;
	lnode tmp = {
		.value = value,
		.list = INIT_LIST_HEAD(head->head),
	};
	*node = tmp;
	dlist_add(&node->list, &head->head);
}

/*
 * 从头部删除一个节点
 */
void *list_del(struct list *head)
{
	if ( dlist_empty(&head->head) )
		return VALUE_INVALID;
	struct dlist_head * node = head->head.next;
	struct lnode * lnode = container_of(node, struct lnode, list); 
	void * value = lnode->value;
	dlist_del(head->head.next);
	free(lnode);
	return value;
}

void list_destroy(struct list *head)
{

}
