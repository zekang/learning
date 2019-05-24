#include "stdafx.h"
#include <stdlib.h>
#include "linklist.h"


LinkList* listCreate()
{
	pInnerLinkList list = (pInnerLinkList)malloc(sizeof(InnerLinkList));
	if(list != NULL){
		list->length = 0;
		list->header.next = NULL;
	}
	return list;
}
pLinkNode listDelete(pLinkList list, int pos)
{
	if (list == NULL || pos < 0) {
		return NULL;
	}
	pInnerLinkList iList = (pInnerLinkList)list;
	if (pos >= iList->length) {
		return NULL;
	}
	pLinkNode current = &(iList->header);
	pLinkNode ret = NULL;
	int i;
	for (i = 0; i < pos && current->next; i++) {
		current = current->next;
	}
	ret = current->next;
	current->next = ret->next;
	iList->length--;

	return ret;
}
pLinkNode listGet(pLinkList list, int pos)
{
	if (list == NULL || pos < 0) {
		return NULL;
	}
	pInnerLinkList iList = (pInnerLinkList)list;
	if (pos >= iList->length) {
		return NULL;
	}
	pLinkNode current = &(iList->header);

	for (int i = 0; i < pos && current->next; i++) {	
		current = current->next;
	}
	return current->next;
}
void listReset(pLinkList list)
{
	if (list == NULL) {
		return;
	}
	pInnerLinkList iList = (pInnerLinkList)list;

	iList->header.next = NULL;
	iList->length = 0;
}
int  listLength(pLinkList list)
{
	if (list == NULL) {
		return 0 ;
	}
	pInnerLinkList iList = (pInnerLinkList)list;
	return iList->length;
}

void listDestroy(pLinkList list)
{
	if (list == NULL) {
		return;
	}
	free(list);
}

int listInsert(pLinkList list, pLinkNode node, int pos)
{
	if (list == NULL || pos < 0 || node == NULL) {
		return -1;
	}
	pInnerLinkList iList = (pInnerLinkList)list;

	pLinkNode current = &(iList->header);
	int i;
	for ( i= 0; i < pos && current->next; i++) {
		current = current->next;
	}
	node->next = current->next;
	current->next = node;
	
	iList->length++;
	return 0;
}