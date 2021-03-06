// 循环链表.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdlib.h>
#include "CircleList.h"
typedef struct _Teacher {
	ListNode node;
	int age;
}Teacher;

void test01()
{
	pCircleList list = CircleList_Create();
	Teacher t1, t2, t3, t4;
	t1.age = 1;
	t2.age = 2;
	t3.age = 3;
	t4.age = 4;
	CircleList_Insert(list, (pListNode)&t1, 0);
	CircleList_Insert(list, (pListNode)&t2, 0);
	CircleList_Insert(list, (pListNode)&t3, 0);
	CircleList_Insert(list, (pListNode)&t4, 0);
	for (int i = 0; i < CircleList_Length(list); i++) {
		Teacher* t = (Teacher *)CircleList_Get(list, i);
		printf("%d\n", t->age);
	}
	printf("cursor\n");
	CircleList_CursorReset(list);
	for (int i = 0; i < CircleList_Length(list) * 2; i++) {
		Teacher* t = (Teacher *)CircleList_Current(list);
		printf("%d\n", t->age);
		CircleList_Next(list);
	}
	printf("list length:%d\n", CircleList_Length(list));
	CircleList_Clear(list);
	printf("list length:%d\n", CircleList_Length(list));
	CircleList_Destroy(list);
}


void test02()
{
	pCircleList list = CircleList_Create();
	Teacher t1, t2, t3, t4;
	t1.age = 1;
	t2.age = 2;
	t3.age = 3;
	t4.age = 4;
	CircleList_Insert(list, (pListNode)&t1, CircleList_Length(list));
	CircleList_Insert(list, (pListNode)&t2, CircleList_Length(list));
	CircleList_Insert(list, (pListNode)&t3, CircleList_Length(list));
	CircleList_Insert(list, (pListNode)&t4, CircleList_Length(list));
	for (int i = 0; i < CircleList_Length(list); i++) {
		Teacher* t = (Teacher *)CircleList_Get(list, i);
		printf("%d\n", t->age);
	}
	printf("cursor\n");
	CircleList_CursorReset(list);

	CircleList_Next(list);
	CircleList_Next(list);
	CircleList_DeleteNode(list, CircleList_Current(list));

	for (int i = 0; i < CircleList_Length(list) * 2; i++) {
		Teacher* t = (Teacher *)CircleList_Current(list);
		printf("%d\n", t->age);
		CircleList_Next(list);
	}
	printf("list length:%d\n", CircleList_Length(list));
	CircleList_Clear(list);
	printf("list length:%d\n", CircleList_Length(list));
	CircleList_Destroy(list);
}

void test03()
{
	int n = 3, m = 3;
	pCircleList list = CircleList_Create();
	Teacher* teacher = NULL;
	for (int i = 0; i < n; i++) {
		teacher = (Teacher *)malloc(sizeof(Teacher));
		memset(teacher, 0, sizeof(Teacher));
		teacher->age = i + 1;
		CircleList_Insert(list, (pListNode)teacher, CircleList_Length(list));
	}
	int count = 1;
	while (CircleList_Length(list) > 0) {
		if (count == m) {
			teacher  = (Teacher*) CircleList_Current(list);
			printf("%d\n", teacher->age);
			CircleList_DeleteNode(list, (pListNode)teacher);
			free(teacher);
			count = 1;
		}else{
			count++;
			CircleList_Next(list);
		}
	}
}

int main()
{
	test03();
	system("pause");
    return 0;
}

