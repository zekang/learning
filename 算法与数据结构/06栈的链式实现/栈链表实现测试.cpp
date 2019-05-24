#include <stdio.h>
#include <stdlib.h>
#include "LinkStack.h"

static void test()
{
	pLinkStack stack = LinkStack_Create();
	for (int i = 0; i < 10; i++) {
		LinkStack_Push(stack, (pElement)i);
	}
	printf("Length:%d\n", LinkStack_Length(stack));
	printf("Top:%d\n", (int)LinkStack_Top(stack));
	while (LinkStack_Length(stack) > 0) {
		int node = (int)LinkStack_Top(stack);
		printf("%d ", node);
		LinkStack_Pop(stack);
	}
	printf("\n");
	printf("Length:%d\n", LinkStack_Length(stack));
	LinkStack_Clear( stack);
	LinkStack_Destroy(stack);
}


int main(int argc, char* argv[])
{
	test();
	system("pause");
	return 0;
}