%{
	#include<stdio.h>
	#include<stdlib.h>
	int chars,words,lines;
	int total_char,total_word,total_line;
%}
%option noyywrap
%%
[-a-zA-Z_]+ {
	words++;
	chars += strlen(yytext);
}
\n {
	chars++;
	lines++;
}
. {
	chars++;
}
%%
int main(int argc,char *argv[])
{
	
	FILE *fp=NULL;
	int i = 1;
	chars = words = lines = 0;			
	total_char = total_word = total_line = 0;
	if(argc == 1){			
		yylex();
		printf("c:%d,w:%d,l:%d\n",chars,words,lines);
		return 0;
	}
	while(i<argc){
		if(fp != NULL){
			fclose(fp);
		}
		fp = fopen(argv[i],"r");	
		if(fp == NULL){
			i++;
			perror("fopen");
			continue;
		}
		yyrestart(fp);
		yylex();
		total_char += chars;
		total_word += words;
		total_line += lines;
		printf("%s c:%d,w:%d,l:%d\n",argv[i],chars,words,lines);
		chars = words = lines = 0;			
		i++;
	}
	if(fp != NULL){
			fclose(fp);
	}
	printf("total c:%d,w:%d,l:%d\n",total_char,total_word,total_line);
	return 0;
}
