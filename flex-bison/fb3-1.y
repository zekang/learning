%{
	#include<stdio.h>
	#include<stdlib.h>
	#include<stdarg.h>
	#include "fb3-1.h"
%}
%union {
	struct ast *a ;
	double d;
}

%token <d> NUMBER
%token EOL

%type <a> exp factor term

%%
calclist :
|calclist exp EOL {
	printf("=%4.4g\n",eval($2));
	treefree($2);
	printf("> ");
}
| calclist EOL {
	printf("> ");
}
;
exp: factor
	| exp '+' factor {
		$$ = newast('+',$1,$3);
	}
	| exp '-' factor {
		$$ = newast('-',$1,$3);
	}
	;
factor:term
	| factor '*' term {
		$$ = newast('*',$1,$3);
	}
	| factor '/' term {
		$$ = newast('/',$1,$3);
	}
	;
term: NUMBER{
	$$ = newnum($1);
	}
	| '|' term {
	$$ = newast('|',$2,NULL);
	}
	| '(' exp ')' {
		$$ = $2;
	}
	| '-' term {
		$$ = newast('M',$2,NULL);
	}
	;
%%

struct ast *newast(int nodetype,struct ast *l,struct ast *r)
{
	struct ast *a = (struct ast *)malloc(sizeof(struct ast));
	if(!a){
		yyerror("malloc error");
		exit(-1);
	}
	a->nodetype = nodetype;
	a->l = l;
	a->r = r;
	return a;
}

struct ast *newnum(double val)
{
	struct numval *num = (struct numval *) malloc(sizeof(struct numval));
	if(!num){
		yyerror("malloc error");
		exit(-1);
	}
	num->nodetype = 'K';
	num->number = val;

	return (struct ast *) num;
}

double eval(struct ast *a)
{
	double v ;
	switch(a->nodetype){
		case 'K':
			v = ((struct numval*) a)->number;
			break;
		case '+' :
			v = eval(a->l) + eval(a->r);
			break;
		case '-' :
			v = eval(a->l) - eval(a->r);
			break;
		case '*' :
			v = eval(a->l) * eval(a->r);
			break;
		case '/' :
			v = eval(a->l) / eval(a->r);
			break;
		case '|' :
			v = eval(a->l);
			if(v < 0){
				v = -v;
			}
			break;
		case 'M' :
			v = -1 * eval(a->l);
			break;
		default :
			printf("internal error ,bad nodetype :%d\n",a->nodetype);
			break;
	}
	return v;
}

void treefree(struct ast *a)
{
	if(!a){
		return ;
	}
	switch(a->nodetype){
		case '+' :
		case '-' :
		case '*' :
		case '/' :
			treefree(a->r);
		case '|' :
		case 'M' :
			treefree(a->l);
		case 'K':
			free(a);
			break;
		default :
			printf("internal error ,bad nodetype :%d\n",a->nodetype);
			break;
	}
}

void yyerror(char *s,...)
{
	va_list ap;
	va_start(ap,s);
	fprintf(stderr,"%d:error:",yylineno);
	vfprintf(stderr,s,ap);
	va_end(ap);
}

int main(int argc,char *argv[])
{
	printf("> ");
	return yyparse();
}
