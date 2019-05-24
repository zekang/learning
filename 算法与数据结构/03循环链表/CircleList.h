#pragma once

//����ڵ�
typedef struct _CircleListNode{
	struct _CircleListNode* next;
}ListNode,*pListNode;

//������
typedef void CircleList, *pCircleList;

//����ṹ
typedef struct _InnerCircleList {
	ListNode header;
	int length;
	pListNode cursor;
}InnerCircleList,*pInnerCircleList;
#ifdef __cplusplus
extern "C" {
#endif
	//��ʼ������
	pCircleList CircleList_Create();
	//��������
	void CircleList_Destroy(pCircleList list);
	//�������
	void CircleList_Clear(pCircleList list);
	//��������
	void CircleList_Insert(pCircleList list, pListNode node, int pos);
	//ɾ��ָ��λ������Ԫ��
	pListNode CircleList_Delete(pCircleList list, int pos);
	//��ȡָ��λ������Ԫ��
	pListNode CircleList_Get(pCircleList list, int pos);
	//����ĳ���
	int CircleList_Length(pCircleList list);

	//����Ԫ��ɾ��
	pListNode CircleList_DeleteNode(pCircleList list, pListNode node);
	//�α�����
	void CircleList_CursorReset(pCircleList list);
	//��ȡ�α�ָ���Ԫ��
	pListNode CircleList_Current(pCircleList list);
	//�����α�
	void CircleList_Next(pCircleList list);
#ifdef __cplusplus
}
#endif