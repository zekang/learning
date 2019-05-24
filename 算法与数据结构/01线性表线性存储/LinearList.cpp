#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinearList.h"

#define ENSURE_LIST_NO_RETURN() \
	if (list == NULL) {\
		return;\
	}

#define ENSURE_LIST(value) \
	if (list == NULL) {\
		return value ;\
	}

#define CONVERT_INNER_LIST(variable) pInnerLinearList variable = (pInnerLinearList)list

pLinearList LinearList_Create(int capacity)
{
	if (capacity < 1) {
		return NULL;
	}
	pInnerLinearList list = (pInnerLinearList)malloc(sizeof(InnerLinearList));
	if (list == NULL) {
		return NULL;
	}
	list->capacity = capacity;
	list->length = 0;
	list->buckets = (LinearNode**)malloc(sizeof(LinearNode*) * capacity);
	if (list->buckets == NULL) {
		free(list);
		return NULL;
	}
	memset(list->buckets, 0, sizeof(LinearNode*) * capacity);
	return list;
}

void LinearList_Destory(pLinearList list)
{
	ENSURE_LIST_NO_RETURN();
	CONVERT_INNER_LIST(iList);
	free(iList->buckets);
	free(iList);
}

void LinearList_Clear(pLinearList list)
{
	ENSURE_LIST_NO_RETURN();
	CONVERT_INNER_LIST(iList);
	iList->length = 0;
	memset(iList->buckets, 0, sizeof(LinearNode*) * iList->capacity);
}
int LinearList_Insert(pLinearList list, pLinearNode node, int pos)
{
	ENSURE_LIST(-1);
	if (node == NULL || pos < 0) {
		return -2;
	}
	CONVERT_INNER_LIST(iList);
	if (iList->length == iList->capacity) {
		return -3;
	}
	if (pos >= iList->capacity) {
		return -4;
	}
	//把要插入位置的元素及后继元素分别向后移动1个单位
	for (int i = iList->length-1; i>=pos ; i--) {
		iList->buckets[i + 1] = iList->buckets[i];
	}
	iList->length++;
	iList->buckets[pos] = node;
	return 0;
}

pLinearNode LinearList_Get(pLinearList list, int pos)
{
	ENSURE_LIST(NULL);
	CONVERT_INNER_LIST(iList);
	if ( pos < 0 || pos>=iList->length) {
		return NULL;
	}
	return iList->buckets[pos];
}

pLinearNode LinearList_Delete(pLinearList list, int pos)
{
	ENSURE_LIST(NULL);
	CONVERT_INNER_LIST(iList);
	if (pos < 0 || pos >= iList->length) {
		return NULL;
	}
	pLinearNode ret = iList->buckets[pos];
	//把要删除位置的元素及后继元素分别向前移动1个单位
	for (int i = pos; i < iList->length; i++) {
		iList->buckets[i] = iList->buckets[i + 1];
	}
	iList->length--;
	return ret;
}

int	LinearList_Length(pLinearList list)
{
	ENSURE_LIST(0);
	CONVERT_INNER_LIST(iList);
	return iList->length;
}

int	LinearList_Capacity(pLinearList list)
{
	ENSURE_LIST(0);
	CONVERT_INNER_LIST(iList);
	return iList->capacity;
}