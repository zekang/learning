#pragma once
#include "linklist.h"
typedef void LinkStack, *pLinkStack;
typedef void Element, *pElement;
typedef struct _stackNode
{
	LinkNode node;
	void* data;
} StackNode,*pStackNode;
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	pLinkStack LinkStack_Create();
	void LinkStack_Destroy(pLinkStack stack);
	void LinkStack_Clear(pLinkStack stack);
	int	 LinkStack_Push(pLinkStack stack, pElement ele);
	pElement LinkStack_Top(pLinkStack stack);
	pElement LinkStack_Pop(pLinkStack stack);
	int LinkStack_Length(pLinkStack stack);
#ifdef __cplusplus
}
#endif // __cplusplus