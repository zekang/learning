%{
#include<stdio.h>
#include<stdlib.h>
%}
%token NUMBER 
%token ADD SUB MUL DIV ABS
%token EOL LB RB
%%
calclist: 
		| calclist expr EOL {
			printf("=%d\n",$2);
		}
		;
expr: factor 		  { $$ = $1 ;}
	| expr ADD factor { $$ = $1 + $3 ;}
	| expr SUB factor { $$ = $1 - $3 ;}
	;
factor:term 		  { $$ = $1 ;}
	| factor MUL term { $$ = $1 * $3 ;}
	| factor DIV term { $$ = $1 / $3 ;}
	;
term: NUMBER   { $$ = $1 ;}
	| ABS term { $$ = $2 > 0 ? $2 : -$2;}
	| LB expr RB { $$ = $2 ;}
	;
%%
int yyerror(char *err)
{
	fprintf(stderr,"error:%s\n",err);
	return 0;
}
int main(int argc,char *argv[])
{
	yyparse();	
	return 0;
}
