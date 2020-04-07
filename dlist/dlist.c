#include "dlist.h"

void list_init(struct list_head *entry)
{
	entry->prev = entry->next = entry;
}

bool list_empty(struct list_head *entry)
{
	return entry->next == entry;
}

bool list_is_tail(struct list_head *entry, struct list_head *head)
{
	return entry == head;
}

bool list_is_head(struct list_head *entry, struct list_head *head)
{
	return entry->prev == head;
}

void __list_add(struct list_head *entry, struct list_head *prev, struct list_head *next)
{
	entry->next = next;
	entry->prev = prev;
	next->prev = entry;
	prev->next = entry;
}

void list_add(struct list_head *entry, struct list_head *head)
{
	__list_add(entry, head, head->next);
}

void __list_del(struct list_head *prev, struct list_head *next)
{
	prev->next = next;
	next->prev = prev;
}

void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
}
