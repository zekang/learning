#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DoubleList.h"
struct Teacher {
	DoubleNode node;
	int age;
};

static void test()
{
	pDoubleList list = DoubleList_Create();
	int i, count = 10;
	Teacher* teacher = NULL;
	for (i = 0; i < count; i++) {
		teacher = (Teacher *)malloc(sizeof(struct Teacher));
		memset(teacher, 0,sizeof(struct Teacher));
		teacher->age = 30 + i;
		DoubleList_Insert(list, (pDoubleNode)teacher, 0);
	}
	printf("Length:%d\n", DoubleList_GetLength(list));

	for (i = 0; i < DoubleList_GetLength(list); i++) {
		teacher = (Teacher*)DoubleList_Get(list,i);
		printf("%d ", teacher->age);
	}
	printf("\n");

	printf("current:%d\n", ((Teacher*)DoubleList_Current(list))->age);
	
	teacher = (Teacher*)DoubleList_Delete(list, 5);
	printf("delete:%d\n", teacher->age);
	free(teacher);
	teacher = NULL;
	printf("Length:%d\n", DoubleList_GetLength(list));

	DoubleList_Next(list);
	DoubleList_Next(list);
	printf("current:%d\n", ((Teacher*)DoubleList_Current(list))->age);
	DoubleList_Prev(list);
	printf("current:%d\n", ((Teacher*)DoubleList_Current(list))->age);
	while (DoubleList_GetLength(list) > 0) {
		teacher = (Teacher*)DoubleList_Delete(list, 0);
		printf("delete:%d ", teacher->age);
		free(teacher);
		teacher = NULL;
	}
	printf("Length:%d\n", DoubleList_GetLength(list));
	DoubleList_Clear(list);
	DoubleList_Destroy(list);
}


int main(int argc, char** argv)
{
	test();
	system("pause");
	return 0;
}