#pragma once

//链表节点
typedef struct _CircleListNode{
	struct _CircleListNode* next;
}ListNode,*pListNode;

//链表句柄
typedef void CircleList, *pCircleList;

//链表结构
typedef struct _InnerCircleList {
	ListNode header;
	int length;
	pListNode cursor;
}InnerCircleList,*pInnerCircleList;
#ifdef __cplusplus
extern "C" {
#endif
	//初始化链表
	pCircleList CircleList_Create();
	//销毁链表
	void CircleList_Destroy(pCircleList list);
	//清空链表
	void CircleList_Clear(pCircleList list);
	//插入链表
	void CircleList_Insert(pCircleList list, pListNode node, int pos);
	//删除指定位置链表元素
	pListNode CircleList_Delete(pCircleList list, int pos);
	//获取指定位置链表元素
	pListNode CircleList_Get(pCircleList list, int pos);
	//链表的长度
	int CircleList_Length(pCircleList list);

	//根据元素删除
	pListNode CircleList_DeleteNode(pCircleList list, pListNode node);
	//游标重置
	void CircleList_CursorReset(pCircleList list);
	//获取游标指向的元素
	pListNode CircleList_Current(pCircleList list);
	//下移游标
	void CircleList_Next(pCircleList list);
#ifdef __cplusplus
}
#endif