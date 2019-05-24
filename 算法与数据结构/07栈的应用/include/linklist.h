#pragma once
#ifndef _H_LINK_LIST
#define _H_LINK_LIST
typedef void LinkList, *pLinkList;

typedef struct _LinkNode {
	struct _LinkNode* next;
}LinkNode,*pLinkNode;

typedef struct  _innerLinkList {
	int length;
	LinkNode header;
}InnerLinkList,*pInnerLinkList;

pLinkList listCreate();
pLinkNode listDelete(pLinkList list, int pos);
pLinkNode listGet(pLinkList list, int pos);
void listReset(pLinkList list);
int  listLength(pLinkList list);
void listDestroy(pLinkList list);
int listInsert(pLinkList list, pLinkNode node, int pos);
#endif