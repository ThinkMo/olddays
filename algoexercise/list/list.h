#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>

typedef struct Element_{
	void *data;
	struct Element_ *next;
}Element;

typedef struct List_{
	size_t size;
	Element *head;
	Element *tail;
	void (*destory)(void *data); 
	void (*match)(void *key1,void *key2);
}List;

void List_init(List *list,void (*destory)(void *data));

void List_destory(List *list);

int List_inst_next(List *list,Element *elmt,void *data);

int List_remv_next(List *List,Element *elmt,void **data);

#define list_size(list) ((list)->size)

#endif
