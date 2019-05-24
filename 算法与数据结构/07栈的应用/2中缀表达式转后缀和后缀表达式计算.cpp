#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "include\LinkStack.h"

#define TOKEN_PLUS 1
#define TOKEN_MINUS 2
#define TOKEN_MUL 3
#define TOKEN_DIV 4
#define TOKEN_LEFT_BRACKET 5
#define TOKEN_RIGHT_BRACKET 6
#define TOKEN_NUMBER 7
#define TOKEN_EOF 0

static char tables[128] = { 0 };
static void init()
{
	tables[TOKEN_PLUS] = '+';
	tables[TOKEN_MINUS] = '-';
	tables[TOKEN_MUL] = '*';
	tables[TOKEN_DIV] = '/';
	tables[TOKEN_LEFT_BRACKET] = '(';
	tables[TOKEN_RIGHT_BRACKET] = ')';
	tables[TOKEN_NUMBER] = 'N';

	tables[TOKEN_PLUS+10] = 0;
	tables[TOKEN_MINUS+10] = 0;
	tables[TOKEN_MUL+10] = 1;
	tables[TOKEN_DIV+10] = 1;



	tables['+'] = TOKEN_PLUS;
	tables['-'] = TOKEN_MINUS;
	tables['*'] = TOKEN_MUL;
	tables['/'] = TOKEN_DIV;
	tables['('] = TOKEN_LEFT_BRACKET;
	tables[')'] = TOKEN_RIGHT_BRACKET;
}

static int token(const char* str,int* data,const char** pos)
{
	const char* p = str;
	if (*pos != NULL) {
		p = *pos;
	}
	char buf[32] = { 0 };
	int index = 0;
	while (*p) {
		if (isspace(*p)) {
			p++;
			continue;
		}
		if (isdigit(*p)) {
			buf[index] = *p;
			if (!isdigit(*(p + 1))) {
				if (index == 0) {
					*data = *p - '0';
				}
				else {
					*data = atoi(buf);
				}
				*pos = p + 1;
				return TOKEN_NUMBER;
			}
			else {
				index++;
			}
		}
		else 
		{
			*pos = p + 1;
			return tables[*p];
		}
		p++;
	}
	return TOKEN_EOF;
}

static void test_token(char* str)
{
	//const char* str = "17*8/(25-20)*3";
	const char* pos = NULL;
	int data = 0, result;
	do {
		result = token(str, &data, &pos);
		if (result == TOKEN_NUMBER) {
			printf("%d", data);
		}
		else {
			printf("%c", tables[result]);
		}
		printf("\n");
	} while (result != TOKEN_EOF);
}

/**
*中缀表达式a + b*c + (d * e + f) * g，其转换成后缀表达式则为a b c * + d e * f  + g * +。
转换过程需要用到栈，具体过程如下：
1）如果遇到操作数，我们就直接将其输出。
2）如果遇到操作符，则我们将其放入到栈中，遇到左括号时我们也将其放入栈中。
3）如果遇到一个右括号，则将栈元素弹出，将弹出的操作符输出直到遇到左括号为止。注意，左括号只弹出并不输出。
4）如果遇到任何其他的操作符，如（“+”， “*”，“（”）等，从栈中弹出元素直到遇到发现更低优先级的元素(或者栈为空)为止。弹出完这些元素后，才将遇到的操作符压入到栈中。有一点需要注意，只有在遇到" ) "的情况下我们才弹出" ( "，其他情况我们都不会弹出" ( "。
5）如果我们读到了输入的末尾，则将栈中所有元素依次弹出。
*/
typedef struct _dataNode {
	int type;
	double value;
}DataNode,*pDataNode;

static void convert(const char* str,char* out,int out_size)
{
	pLinkStack stack = LinkStack_Create();
	const char* pos = NULL;
	int data = 0, result;
	bool flag;
	pDataNode dataNode = NULL;
	char buf[32] = { 0 };
	do {
		result = token(str, &data, &pos);
		if (result == TOKEN_EOF) {
			break;
		}
		if (result == TOKEN_NUMBER) {
			sprintf_s(buf, "%d ", data);
			strcat_s(out, out_size-strlen(out), buf);
		}
		else {
			if (LinkStack_Length(stack) == 0 || result == TOKEN_LEFT_BRACKET) {
				dataNode = (pDataNode)malloc(sizeof(DataNode));
				dataNode->type = result;
				dataNode->value = tables[result];
				LinkStack_Push(stack, (pElement)dataNode);
			}
			else if(result == TOKEN_RIGHT_BRACKET) { 
				flag = true;
				do {
					dataNode =(pDataNode) LinkStack_Pop(stack);
					if (dataNode == NULL) {
						break;
					}
					if (dataNode->type != TOKEN_LEFT_BRACKET) {
						sprintf_s(buf, "%c ", (int)dataNode->value);
						strcat_s(out, out_size - strlen(out), buf);
					}
					else {
						flag = false;
					}
					free(dataNode);
					dataNode = NULL;
				} while (flag && LinkStack_Length(stack)>0);
			}
			else {
				flag = true;
				do {
					dataNode = (pDataNode)LinkStack_Top(stack);
					if (dataNode->type == TOKEN_LEFT_BRACKET || tables[result + 10] > tables[dataNode->type + 10]) {
						flag = false;
					}
					else {
						sprintf_s(buf, "%c ", (int)dataNode->value);
						strcat_s(out, out_size - strlen(out), buf);
						LinkStack_Pop(stack);
						free(dataNode);
						dataNode = NULL;
					}
				} while (flag && LinkStack_Length(stack) > 0);
				dataNode = (pDataNode)malloc(sizeof(DataNode));
				dataNode->type = result;
				dataNode->value = tables[result];
				LinkStack_Push(stack, (pElement)dataNode);
			}
		}
	} while (1);

	while (LinkStack_Length(stack) > 0) {
		dataNode = (pDataNode)LinkStack_Top(stack);
		if (dataNode->type == TOKEN_NUMBER) {
			//printf("%d ", dataNode->value);
			sprintf_s(buf, "%d ", (int)dataNode->value);
			strcat_s(out, out_size - strlen(out), buf);
		}
		else {
			//printf("%c ", dataNode->value);
			sprintf_s(buf, "%c ", (int)dataNode->value);
			strcat_s(out, out_size - strlen(out), buf);
		}
		LinkStack_Pop(stack);
		free(dataNode);
		dataNode = NULL;
	}
	LinkStack_Destroy(stack);
}

static void test_convert()
{
	const char* str = "17*8/(25-20)*3";
	char out[128] = { 0 };
	puts(str); 
	convert(str,out,sizeof(out)-1);
	puts(out);
	test_token(out);
}

static double calculate(const char*str)
{
	char out[128] = { 0 };
	const char* pos = NULL;
	int data = 0, result;
	double ret = 0,left,right;
	puts(str);
	convert(str, out, sizeof(out) - 1);
	puts(out);
	pDataNode dataNode = NULL;
	pLinkStack stack = LinkStack_Create();
	do {
		result = token(out, &data, &pos);
		if (result == TOKEN_EOF) {
			break;
		}
		if (result == TOKEN_NUMBER) {
			dataNode = (pDataNode)malloc(sizeof(DataNode));
			dataNode->type = result;
			dataNode->value = data;
			LinkStack_Push(stack, (pElement)dataNode);
		}
		else {
			dataNode = (pDataNode)LinkStack_Pop(stack);
			right = dataNode->value;
			free(dataNode);
			dataNode = (pDataNode)LinkStack_Pop(stack);
			left = dataNode->value;
			free(dataNode);
			switch (result) {
			case TOKEN_PLUS:
				ret = left + right;
				break;
			case TOKEN_MINUS:
				ret = left - right;
				break;
			case TOKEN_MUL:
				ret = left * right;
				break;
			case TOKEN_DIV:
				ret = left / right;
				break;
			}
			dataNode = (pDataNode)malloc(sizeof(DataNode));
			dataNode->type = TOKEN_NUMBER;
			dataNode->value = ret;
			LinkStack_Push(stack, (pElement)dataNode);
		}
	} while (1);
	dataNode = (pDataNode)LinkStack_Pop(stack);
	ret  = dataNode->value;
	free(dataNode);
	LinkStack_Destroy(stack);
	return ret;
}
static void test_calculate()
{
	const char* str = "17*8/(25-20)*3";
	double ret = calculate(str);
	printf("%f", ret);
}
int main(int argc, char* argv[])
{
	init();
//	test_token((char *)"17*8/(25-20)*3");
//	test_convert();
	test_calculate();
	putchar('\n');
	system("pause");
	return EXIT_SUCCESS;
}