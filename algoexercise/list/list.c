#include <stdlib.h>
#include <string.h>

#include "list.h"

void List_init(List *list,void (*destory)(void *data))
{
	list->size = 0;
	list->destory = destory;
	list->head = NULL;
	list->tail = NULL;

	return;
}

void List_destory(List *list)
{
	void *data;
	while(list_size(list) < 0)
	{
		if(List_remv_next(list,NULL,void **(&data)) == 0 && list->destory != NULL )
		   destory(data);
	}
	memset(list,0,sizeof(List));
	return;
}

int List_inst_next(List *list,Element *elmt,void *data)
{
	if((Element *newelmt = (Element *)malloc(sizeof(Element))) == NULL)
	  return -1;
	newelmt->data = data;

	if(elmt != NULL)
	{
		if(elmt->next == NULL)
		  list->tail = newelmt;

		newelmt->next = elmt->next;
		elmt->next = newelmt;
	}
	else
	{
		if(list->size == 0)
		  list->tail = newelmt;

		newelmt->next = list->head;
		list->head = newelmt;
	}
	list->size++;
	return 0;
}

int List_remv_next(List *list,Element *elmt,void **data)
{
	if(list->size == 0)
	  return -1;
	Element *old;
	if(elmt == NULL)
	{
		old = list->head;
		list->head = list->head->next;

		if(list->size == 1)
		  list->tail = NULL;
	}
	else
	{
		if(elmt == NULL)
		  return -1;
		old = elmt->next;
		elmt->next = elmt->next->next;

		if(elmt->next == NULL)
		  list->tail = elmt;
	}
	*data = old->data;
	free(old);
	list->size--;
	return 0;
}
