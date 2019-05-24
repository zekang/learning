#include <stdio.h>
#include <stdlib.h>
#include "DoubleList.h"
#define CHECK_DOUBLELIST_HANDLE(variable) if(list == NULL){ return ;} pInnerDoubleList variable = (pInnerDoubleList) list

#define CHECK_DOUBLELIST_HANDLE_RETURN(value,variable) if(list == NULL) {return value;} pInnerDoubleList variable = (pInnerDoubleList) list

pDoubleList DoubleList_Create()
{
	pInnerDoubleList list = (pInnerDoubleList)malloc(sizeof(InnerDoubleList));
	if (list == NULL) {
		return NULL;
	}
	DoubleList_Clear(list);
	return list;
}

void DoubleList_Destroy(pDoubleList list)
{
	CHECK_DOUBLELIST_HANDLE(iList);
	free(iList);
}

void DoubleList_Clear(pDoubleList list)
{
	CHECK_DOUBLELIST_HANDLE(iList);
	iList->length = 0;
	iList->cursor = NULL;
	iList->header.next = NULL;
	iList->header.prev = NULL;
}

int	DoubleList_GetLength(pDoubleList list)
{
	CHECK_DOUBLELIST_HANDLE_RETURN(0, iList);
	return iList->length;
}

pDoubleNode DoubleList_Get(pDoubleList list, int pos)
{
	CHECK_DOUBLELIST_HANDLE_RETURN(NULL, iList);
	if ( pos < 0 || pos>iList->length ) {
		return NULL;
	}
	pDoubleNode current = & iList->header;
	for (int i = 0; i < pos && current->next != NULL; i++) {
		current = current->next;
	}
	return current->next;
}


pDoubleNode DoubleList_Delete(pDoubleList list, int pos)
{
	CHECK_DOUBLELIST_HANDLE_RETURN(NULL, iList);
	if (pos < 0 || pos>iList->length) {
		return NULL;
	}
	pDoubleNode current = &iList->header;
	pDoubleNode ret = NULL,prev=NULL,next=NULL;
	for (int i = 0; i < pos && current->next != NULL; i++) {
		current = current->next;
	}
	ret = current->next;
	prev = ret->prev;
	next = ret->next;
	if (current == list) { //删除的为头接点
		iList->header.next = next;
	}

	if (prev != NULL) {
		prev->next = next;
	}
	
	if(next !=NULL){
		next->prev = prev;
	}

	if (iList->cursor == ret) {
		iList->cursor = ret->next;
	}
	iList->length--;
	return ret;
}


int	DoubleList_Insert(pDoubleList list, pDoubleNode node, int pos)
{
	CHECK_DOUBLELIST_HANDLE_RETURN(-1, iList);

	if (pos < 0 || node == NULL) {
		return -2;
	}

	pDoubleNode current = &iList->header;
	pDoubleNode prev = NULL, next = NULL;

	for (int i = 0; i < pos && current->next != NULL; i++) {
		current = current->next;
	}
	prev = current;
	next = current->next;

	node->next = next;
	node->prev = prev;
	if (next != NULL) {
		next->prev = node;
	}
	if (current == list) { //从头部插入接点
		node->prev = NULL;
		iList->cursor = node;
		iList->header.next = node;
	}
	iList->length++;
	return 0;
}


pDoubleNode DoubleList_Current(pDoubleList list)
{
	CHECK_DOUBLELIST_HANDLE_RETURN(NULL, iList);
	return iList->cursor;
}


void DoubleList_Next(pDoubleList list)
{
	CHECK_DOUBLELIST_HANDLE(iList);
	if(iList->cursor != NULL){
		iList->cursor = iList->cursor->next;
	}
}


void DoubleList_Prev(pDoubleList list)
{
	CHECK_DOUBLELIST_HANDLE(iList);
	if (iList->cursor != NULL) {
		iList->cursor = iList->cursor->prev;
	}
}

void DoubleList_CursorRest(pDoubleList list)
{
	CHECK_DOUBLELIST_HANDLE(iList);
	iList->cursor = iList->header.next;
}

