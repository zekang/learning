%{
#include "fb1-5.tab.h"
int yylval;
%}
%%
"+" {
	printf("op:%d\n",ADD);
	return ADD;
}
"-" {
	return SUB;
}
"*" {
	return MUL;
}
"/" {
	return DIV;
}
"|" {
	return ABS;
}
[0-9]+ {
	yylval = atoi(yytext);
	printf("number:%d\n",yylval);
	return NUMBER;
}
[ \t] {
}
\n {
	return EOL;
}
. {
	printf("unknown character:%s\n",yytext);
}
%%
