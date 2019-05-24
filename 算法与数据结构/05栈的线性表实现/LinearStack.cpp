#include <stdio.h>
#include <stdlib.h>
#include "LinearStack.h"
#include "LinearList.h"

pLinearStack LinearStack_Create(int capacity)
{
	return LinearList_Create(capacity);
}

void LinearStack_Clear(pLinearStack stack)
{
	LinearList_Clear(stack);
}

void LinearStack_Destroy(pLinearStack stack)
{
	LinearList_Destory(stack);
}

int LinearStack_Capacity(pLinearStack stack)
{
	return LinearList_Capacity(stack);
}

int LinearStack_Length(pLinearStack stack)
{
	return LinearList_Length(stack);
}

pLinearStackNode LinearStack_Top(pLinearStack stack)
{
	return LinearList_Get(stack,LinearList_Length(stack)-1);
}

pLinearStackNode LinearStack_Pop(pLinearStack stack)
{
	return LinearList_Delete(stack, LinearList_Length(stack) - 1);
}

int LinearStack_Push(pLinearStack stack, pLinearStackNode node)
{
	return LinearList_Insert(stack,(pLinearNode)node,LinearList_Length(stack));
}
