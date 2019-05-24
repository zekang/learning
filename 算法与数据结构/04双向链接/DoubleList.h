#pragma once
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	typedef void DoubleList, *pDoubleList;
	
	typedef struct _DoubleNode{
		struct _DoubleNode* next;
		struct _DoubleNode* prev;
	}DoubleNode, *pDoubleNode;

	typedef struct _InnerDoubleList {
		DoubleNode header;
		int length;
		struct _DoubleNode* cursor;
	}InnerDoubleList,*pInnerDoubleList;

	pDoubleList DoubleList_Create();
	void		DoubleList_Destroy(pDoubleList list);
	void		DoubleList_Clear(pDoubleList list);
	int			DoubleList_GetLength(pDoubleList list);
	pDoubleNode DoubleList_Get(pDoubleList list, int pos);
	pDoubleNode DoubleList_Delete(pDoubleList list, int pos);
	int			DoubleList_Insert(pDoubleList list, pDoubleNode node, int pos);
	pDoubleNode DoubleList_Current(pDoubleList list);
	void		DoubleList_Next(pDoubleList list);
	void		DoubleList_Prev(pDoubleList list);
	void		DoubleList_CursorRest(pDoubleList list);
#ifdef __cplusplus
}
#endif
