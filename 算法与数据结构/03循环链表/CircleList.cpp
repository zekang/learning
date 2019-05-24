#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "CircleList.h"

#define ENSURE_LIST_NO_RETURN() \
	if (list == NULL) {\
		return;\
	}

#define ENSURE_LIST(value) \
	if (list == NULL) {\
		return value ;\
	}

#define CONVERT_INNER_LIST(variable) pInnerCircleList variable = (pInnerCircleList)list
	
//初始化链表
pCircleList CircleList_Create()
{
	pInnerCircleList list = (pInnerCircleList)malloc(sizeof(InnerCircleList));
	assert(list);
	CircleList_Clear(list);
	return list;
}
//销毁链表
void CircleList_Destroy(pCircleList list)
{
	ENSURE_LIST_NO_RETURN();
	free(list);
}
//清空链表
void CircleList_Clear(pCircleList list) 
{
	ENSURE_LIST_NO_RETURN();
	CONVERT_INNER_LIST(iList);
	iList->header.next = NULL;
	iList->cursor = NULL;
	iList->length = 0;
}
void CircleList_Insert(pCircleList list, pListNode node, int pos)
{
	ENSURE_LIST_NO_RETURN();
	if (node == NULL || pos < 0) {
		return;
	}
	CONVERT_INNER_LIST(iList);
	pListNode current = &(iList->header);
	for (int i = 0; current->next != NULL && i < pos; i++) {
		current = current->next;
	}
	node->next = current->next;
	current->next = node;
	//如果是第一个元素
	if (iList->length ==0) {
		node->next = node;
		iList->cursor = node;
	}
	iList->length++;
	//如果是从头部插入
	if (iList->length>1 && current == list) {
		pListNode last = CircleList_Get(list, iList->length - 1);
		last->next = node;
	}
}
//删除指定位置链表元素
pListNode CircleList_Delete(pCircleList list, int pos)
{
	ENSURE_LIST(NULL);
	CONVERT_INNER_LIST(iList);
	if (pos >= iList->length) {
		return NULL;
	}
	pListNode current = &(iList->header);
	pListNode ret = NULL;
	for (int i = 0; current->next != NULL && i < pos; i++) {
		current = current->next;
	}
	ret = current->next;

	current->next = ret->next;
	iList->length--;
	//如果是删除的是第0个元素
	if (current == list && iList->length > 0) {
		pListNode last = CircleList_Get(list, iList->length - 1);
		last->next = current->next;
	}
	if (iList->length == 0) {
		current->next = NULL;
	}
	if (iList->cursor == ret) {
		iList->cursor = ret->next;
	}
	return ret;
}
//获取指定位置链表元素
pListNode CircleList_Get(pCircleList list, int pos) 
{
	ENSURE_LIST(NULL);
	CONVERT_INNER_LIST(iList);
	if (pos >= iList->length) {
		return NULL;
	}
	pListNode current = &(iList->header);
	for (int i = 0; current->next != NULL && i < pos; i++) {
		current = current->next;
	}
	return current->next;
}
//链表的长度
int CircleList_Length(pCircleList list)
{
	ENSURE_LIST(0);
	CONVERT_INNER_LIST(iList);
	return iList->length;
}

//根据元素删除
pListNode CircleList_DeleteNode(pCircleList list, pListNode node)
{
	ENSURE_LIST(NULL);
	CONVERT_INNER_LIST(iList);
	pListNode current = &(iList->header);
	pListNode ret = NULL;
	int pos = -1;
	for (int i = 0; current->next != NULL && i < iList->length; i++) {
		current = current->next;
		if (current == node) {
			pos = i;
			break;
		}
	}
	return CircleList_Delete(list, pos);
}
//游标重置
void CircleList_CursorReset(pCircleList list)
{
	ENSURE_LIST_NO_RETURN();
	CONVERT_INNER_LIST(iList);
	iList->cursor = iList->header.next;
}
//获取游标指向的元素
pListNode CircleList_Current(pCircleList list)
{
	ENSURE_LIST(NULL);
	CONVERT_INNER_LIST(iList);
	return iList->cursor;
}
//下移游标
void CircleList_Next(pCircleList list)
{
	ENSURE_LIST_NO_RETURN();
	CONVERT_INNER_LIST(iList);
	iList->cursor = iList->cursor->next;
}
