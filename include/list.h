/*
 *
 * 基于dlist封装的list，便于支持任意value类型的插入和遍历
 *
 *
 */
#include <stdint.h>
#include "dlist.h"

#define VALUE_INVALID (void *)INT64_MAX

typedef struct list {
	struct dlist_head head;
} list;

void list_init(struct list *head);

void list_add(struct list *head, void *value);

void *list_del(struct list *head);

void list_destroy(struct list *head);
