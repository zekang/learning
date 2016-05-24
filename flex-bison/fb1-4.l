%{
#include<stdio.h>
enum token
{
	ADD = 258,
	MINUS = 259,
	TIMES = 260,
	DIVIDE = 261,
	ABS = 262,
	NUMBER = 263
};
int yyval;
%}

%%	
"+" {
	return ADD;
}
"-" {
	return MINUS;
}
"*" {
	return TIMES;
}
"/" {
	return DIVIDE;
}
"|" {
	return ABS;
}
[0-9]+ {
	yyval = atoi(yytext);
	return NUMBER;
}
[ \t] 
.  
%%
int yywrap()
{
	return 0;
}

int main(int argc,char*argv[])
{
	int token;
	while(token = yylex())
	{
		printf("%d=%s\n",token,yytext);
	}
	return 0;
}
