#pragma once
typedef void LinearList, *pLinearList;

typedef void LinearNode, *pLinearNode;

typedef struct _InnertLinearList {
	int length;
	int capacity;
	LinearNode** buckets;
}InnerLinearList,*pInnerLinearList;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	pLinearList LinearList_Create(int capacity);
	void		LinearList_Destory(pLinearList list);
	void		LinearList_Clear(pLinearList list);
	int			LinearList_Insert(pLinearList list, pLinearNode node, int pos);
	pLinearNode LinearList_Get(pLinearList list, int pos);
	pLinearNode LinearList_Delete(pLinearList list, int pos);
	int			LinearList_Length(pLinearList list);
	int			LinearList_Capacity(pLinearList list);

#ifdef __cplusplus
}
#endif // __cplusplus

