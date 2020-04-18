/// @file main.c
/// @Synopsis
/// @author hnu_hefeng@qq.com
/// @version 0.0.1
/// @date 2020-01-11
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

int main(int argc, char **argv)
{
	list head;
	list_init(&head);

	for ( int64_t i=0;i<10;i++ )
		list_add(&head, (void*)i);

	void *value = NULL;
	while((value=list_del(&head)) != VALUE_INVALID) {
		printf("value=%ld\n", (int64_t)value);
	}

	list_destroy(&head);
  	return 0;
}
