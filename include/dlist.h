#include <stdbool.h>
#include <stdlib.h>


struct list_head {
	struct list_head *prev, *next;
};

void list_init(struct list_head *head);

bool list_empty(struct list_head *head);

bool list_is_tail(struct list_head *entry, struct list_head *head);

bool list_is_head(struct list_head *entry, struct list_head *head);

void list_add(struct list_head *entry, struct list_head *head);

void list_del(struct list_head *entry);

#define INIT_LIST_HEAD(name) {&(name),&(name)}

#define container_of(ptr, type, member) \
	(type*)((char*)(ptr)-(char*)&((type*)0)->member)

#define list_for_each(pos, head) \
	for (; !list_is_tail(pos, head); pos=pos->next)

#define container_for_each(pos, head, member) \
	for (pos=container_of((head)->next, typeof(*pos), member); \
			&pos->member!=head; \
			pos=container_of(pos->member.next, typeof(*pos), member))
