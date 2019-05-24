#pragma once
typedef void LinearStack, *pLinearStack;
typedef void LinearStackNode, *pLinearStackNode;
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	pLinearStack LinearStack_Create(int capacity);
	void LinearStack_Clear(pLinearStack stack);
	void LinearStack_Destroy(pLinearStack stack);
	int LinearStack_Capacity(pLinearStack stack);
	int LinearStack_Length(pLinearStack stack);
	pLinearStackNode LinearStack_Top(pLinearStack stack);
	pLinearStackNode LinearStack_Pop(pLinearStack stack);
	int LinearStack_Push(pLinearStack stack, pLinearStackNode node);
#ifdef __cplusplus
}
#endif // __cplusplus