%option noyywrap
%x IFILE
%{
#include <stdio.h>
#include <stdlib.h>
		struct bufstack {
				struct bufstack *prev;
				YY_BUFFER_STATE bs;
				int lineno;
				char *filename;
				FILE *f;
		} *curbs = NULL;

		char *curfilename;
		int newfile (char *filename);
		int popfile(void);
%}
%%
^"#"[ \t]*include[ \t]*[\"<] {
		BEGIN IFILE;
}
<IFILE>[^ \t\n\">]+ {
	{
		int c;
		while((c=input()) && c != '\n');
	}
	yylineno++;
	if(!newfile(yytext)){
		yyterminate();
	}
	BEGIN INITIAL;
}
<IFILE>.|\n  {
		fprintf(stderr,"%4d bad include line!\n",yylineno);
		yyterminate();
}
<<EOF>> {
		if(!popfile()){
				yyterminate();
		}
}
^. {
		fprintf(yyout,"%4d %s",yylineno,yytext);
}	
^\n {
		fprintf(yyout,"%4d \t in <%s> %s",yylineno,curfilename,yytext);
}
\n {
		fprintf(yyout,"\t in <%s>",curfilename);
		ECHO;
		yylineno++;
}
. {
		ECHO;
}
%%
int main(int argc,char *argv[])
{
		if(argc < 2){
				fprintf(stderr,"need filename\n");
				return -1;
		}
		if(newfile(argv[1])){
				yylex();
		}
		return 0;
}


int newfile(char *filename)
{
		FILE *fp = fopen(filename,"r");
		if(!fp){
				perror(filename);
				return 0;
		}
		struct bufstack *bs = malloc(sizeof(struct bufstack));
		if(!bs){
				perror("malloc");
				return 0;
		}
		if(curbs){
				curbs->lineno = yylineno;
		}
		bs->prev = curbs;
		bs->bs = yy_create_buffer(fp,YY_BUF_SIZE);
		bs->f = fp;
		bs->filename = filename;
		yy_switch_to_buffer(bs->bs);
		curbs = bs;
		yylineno = 1;
		curfilename = filename;
		return 1;
}

int popfile()
{
		struct bufstack *bs = curbs;
		struct bufstack *prev = NULL;

		if(!bs) {
				return 0;
		}
		fclose(bs->f);
		yy_delete_buffer(bs->bs);

		prev = bs->prev;
		free(bs);

		if(!prev){
				return 0;
		}
		yy_switch_to_buffer(prev->bs);
		curbs = prev;
		yylineno = curbs->lineno;
		curfilename = curbs->filename;
		return 1;
}

