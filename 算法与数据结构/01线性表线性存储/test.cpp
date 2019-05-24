#include <stdio.h>
#include <stdlib.h>
#include "LinearList.h"

static void test()
{
	int capacity = 10,i;
	pLinearList list =  LinearList_Create(capacity);
	for (i = 0; i < 8; i++) {
		LinearList_Insert(list, (pLinearNode) (i+1), 0);
	}

	printf("capacity:%d\n", LinearList_Capacity(list));
	printf("length:%d\n", LinearList_Length(list));
	for (i = 0; i < LinearList_Length(list); i++) {
		printf("%d ", (int)LinearList_Get(list, i));
	}
	printf("\n");
	printf("del 4:%d\n", (int)LinearList_Delete(list, 4));

	printf("capacity:%d\n", LinearList_Capacity(list));
	printf("length:%d\n", LinearList_Length(list));
	for (i = 0; i < LinearList_Length(list); i++) {
		printf("%d ", (int)LinearList_Get(list, i));
	}
	printf("\n");

	LinearList_Clear(list);
	printf("capacity:%d\n", LinearList_Capacity(list));
	printf("length:%d\n", LinearList_Length(list));
	LinearList_Destory(list);
}

int main(int argc, char** argv)
{
	test();
	system("pause");
	return 0;
}