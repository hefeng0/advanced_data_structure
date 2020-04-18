#ifndef __DLIST_H__
#define __DLIST_H__

#include <stdbool.h>
#include <stdlib.h>


struct dlist_head {
	struct dlist_head *prev, *next;
};

void dlist_init(struct dlist_head *head);

bool dlist_empty(struct dlist_head *head);

bool dlist_is_tail(struct dlist_head *entry, struct dlist_head *head);

bool dlist_is_head(struct dlist_head *entry, struct dlist_head *head);

void dlist_add(struct dlist_head *entry, struct dlist_head *head);

void dlist_del(struct dlist_head *entry);

#define INIT_LIST_HEAD(name) {&(name),&(name)}

#define container_of(ptr, type, member) \
	(type*)((char*)(ptr)-(char*)&((type*)0)->member)

#define dlist_for_each(pos, head) \
	for (; !dlist_is_tail(pos, head); pos=pos->next)

#define container_for_each(pos, head, member) \
	for (pos=container_of((head)->next, typeof(*pos), member); \
			&pos->member!=head; \
			pos=container_of(pos->member.next, typeof(*pos), member))

#endif
