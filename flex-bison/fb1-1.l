%{
/**
	word count
*/
int chars = 0;
int words = 0;
int lines = 0;
%}
%%
[a-zA-Z]+ {	
		chars += strlen(yytext);
		words++;
}
\n {
	chars++;
	lines++;
}
. { chars++;}
%%

int main(int argc,char *argv[])
{
	yylex();
	printf("chars:%d\t,words:%d\t,lines:%d\n",chars,words,lines);
	return 0;
}
