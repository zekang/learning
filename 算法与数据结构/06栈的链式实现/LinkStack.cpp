#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linklist.h"
#include "LinkStack.h"

pLinkStack LinkStack_Create()
{
	return listCreate();
}

void LinkStack_Destroy(pLinkStack stack)
{
	LinkStack_Clear(stack);
	listDestroy(stack);
}

void LinkStack_Clear(pLinkStack stack)
{
	while (LinkStack_Length(stack) > 0) {
		LinkStack_Pop(stack);
	}
}

int	 LinkStack_Push(pLinkStack stack, pElement ele)
{
	pStackNode node = (pStackNode)malloc(sizeof(StackNode));
	if (node == NULL) {
		return -1;
	}
	memset(node, 0, sizeof(StackNode));
	node->data = ele;
	node->node.next = NULL;
	return listInsert(stack, (pLinkNode)node, 0);
}

pElement LinkStack_Top(pLinkStack stack)
{
	pStackNode node = (pStackNode)listGet(stack, 0);
	if (node != NULL) {
		return node->data;
	}
	return NULL;
}

pElement LinkStack_Pop(pLinkStack stack)
{
	pStackNode node = (pStackNode)listDelete(stack, 0);
	pElement ret = NULL;
	if (node != NULL) {
		ret = node->data;
		free(node);
	}
	return ret;
}

int LinkStack_Length(pLinkStack stack)
{
	return listLength(stack);
}