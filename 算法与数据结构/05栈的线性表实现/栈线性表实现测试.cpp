#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinearStack.h"

static void test()
{
	int capacity = 10;
	pLinearStack stack =  LinearStack_Create(capacity);
	for (int i = 1; i <= 5; i++) {
		LinearStack_Push(stack, (pLinearStackNode)i);
	}
	printf("Capactiy:%d\n", LinearStack_Capacity(stack));
	printf("Length:%d\n", LinearStack_Length(stack));
	while (LinearStack_Length(stack) > 0) {
		int node = (int)LinearStack_Top(stack);
		printf("%d ", node);
		LinearStack_Pop(stack);
	}
	printf("\n");
	printf("Capactiy:%d\n", LinearStack_Capacity(stack));
	printf("Length:%d\n", LinearStack_Length(stack));
	LinearStack_Clear( stack);
	LinearStack_Destroy( stack);
}


int main(int argc, char* argv[])
{
	test();
	system("pause");
	return 0;
}