#include <stdio.h>
#include <stdlib.h>
#include "include\LinkStack.h"

static char tables[128] = { 0 };

static void init()
{
	tables['('] = ')';
	tables['<'] = '>';
	tables['{'] = '}';
	tables['['] = ']';
	tables['\''] = '\'';
	tables['\"'] = '\"';
}

bool isLeft(char c)
{
	return (c == '(' || c == '<' || c == '{'  || c == '[' || c == '\'' || c == '\"');
}

bool isRight(char c)
{
	return (c == ')' || c == '>' || c == '}' || c == ']' || c == '\'' || c == '\"');;
}

bool isMatch(char left,char right)
{
	if (left > sizeof(tables) - 1) {
		return false;
	}
	return tables[left] == right;
}

bool scan(const char* code)
{
	const char* p = code;
	char* c;
	pLinkStack stack = LinkStack_Create();
	bool ret = true;
	while (*p) {
		if (isLeft(*p)) {
			LinkStack_Push(stack, (pElement)p);
		}
		if (isRight(*p)) {
			c = (char *)LinkStack_Pop(stack);
			if (c == NULL || !isMatch(*c, *p)) {
				printf("bad match at pos: %d\n", p - code);
				ret = false;
				break;
			}
		}
		p++;
	}
	if (ret) {
		ret = LinkStack_Length(stack) == 0;
	}
	LinkStack_Destroy(stack);

	return ret;
}


int main1(int argc, char* argv[])
{
	const char* code = "#include <stdio.h> #include <stdlib.h>#include \"include\\LinkStack.h\" int main(int argc, char* argv[])	{		const char* code = \"\";		system(\"pause\");		return EXIT_SUCCESS;	}";
	init();
	if (scan(code)) {
		printf("ok\n");
	}
	else {
		printf("bad\n");
	}
	system("pause");
	return EXIT_SUCCESS;
}