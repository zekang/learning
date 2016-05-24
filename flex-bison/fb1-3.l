%%
"+" {printf("PLUS\n");}
"-" {printf("MINUS\n");}
"*" {printf("TIMES\n");}
"/" {printf("DIVIDE\n");}
[0-9]+ {printf("Number:%s\n",yytext);}
"=" {printf("EQUAL\n");}
[ \t] ;
\n	{printf("NEWLINE\n");}
. {printf("Other\n");}
%%
