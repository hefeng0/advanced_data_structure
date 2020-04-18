#include "dlist.h"

void dlist_init(struct dlist_head *entry)
{
	entry->prev = entry->next = entry;
}

bool dlist_empty(struct dlist_head *entry)
{
	return entry->next == entry;
}

bool dlist_is_tail(struct dlist_head *entry, struct dlist_head *head)
{
	return entry == head;
}

bool dlist_is_head(struct dlist_head *entry, struct dlist_head *head)
{
	return entry->prev == head;
}

void __dlist_add(struct dlist_head *entry, struct dlist_head *prev, struct dlist_head *next)
{
	entry->next = next;
	entry->prev = prev;
	next->prev = entry;
	prev->next = entry;
}

void dlist_add(struct dlist_head *entry, struct dlist_head *head)
{
	__dlist_add(entry, head, head->next);
}

void __dlist_del(struct dlist_head *prev, struct dlist_head *next)
{
	prev->next = next;
	next->prev = prev;
}

void dlist_del(struct dlist_head *entry)
{
	__dlist_del(entry->prev, entry->next);
}
