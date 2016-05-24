%{
#include "fb1-5.tab.h"
int yylval;
%}
%%
"+" { return ADD; }
"-" { return SUB; }
"*" { return MUL; }
"/" { return DIV; }
"|" { return ABS; }
"(" { return LB; }
")" { return RB; }
[0-9]+ {
	yylval = atoi(yytext);
	return NUMBER;
}
[ \t] { }
\n { return EOL; }
. { printf("unknown character:%s\n",yytext); }
%%
