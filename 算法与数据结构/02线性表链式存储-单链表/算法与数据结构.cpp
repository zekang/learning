// 算法与数据结构.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "linklist.h"
#include <stdlib.h>
struct Teacher
{
	LinkNode node;
	int age;
};

void printList(LinkList* list)
{
	printf("listLength:%d\n", listLength(list));
	for (int i = 0; i < listLength(list); i++) {
		Teacher * teacher = (Teacher *)listGet(list, i);
		if(teacher != NULL){
			printf("pos:%d,age=%d\n", i, teacher->age);
		}
	}
}
void linkListTest()
{
	LinkList* list = listCreate();
	if (list == NULL) {
		return;
	}
	printf("listLength:%d\n", listLength(list));
	Teacher t1, t2, t3, t4, t5,t6;
	t1.age = 31;
	t2.age = 32;
	t3.age = 33;
	t4.age = 34;
	t5.age = 35;
	t6.age = 36;
	listInsert(list, (LinkNode*)&t1, 0);
	listInsert(list, (LinkNode*)&t2, 0);
	listInsert(list, (LinkNode*)&t3, 0);
	listInsert(list, (LinkNode*)&t4, 0);
	listInsert(list, (LinkNode*)&t5, 0);
	listInsert(list, (LinkNode*)&t6, 3);
	printList(list);
	
	printf("listLength:%d\n", listLength(list));
	while (listLength(list) > 0) {
		Teacher * teacher = (Teacher *)listDelete(list, 0);
		if (teacher != NULL) {
			printf("age=%d\n", teacher->age);
		}
	}
	printf("listLength:%d\n", listLength(list));
	listReset(list);
	listDestroy(list);
}

int main()
{
	linkListTest();
	system("pause");
    return 0;
}

