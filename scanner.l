%{
#include <math.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
%}
%option noyywrap
DIGIT [0-9]
ID 	[_a-zA-Z][_a-zA-Z0-9]*	

%%

 /* rm whitespace */
[ \r\n\t]+ {
}

 /* Keywords */
 /* TODO: categorize this */
PROGRAM|BEGIN|END|FUNCTION|READ|WRITE|IF|ELSE|ENDIF|WHILE|ENDWHILE|RETURN|INT|VOID|STRING|FLOAT|TRUE|FALSE {
	printf("Token Type: ");
	printf("KEYWORD\n");
	printf("Value: %s\n", yytext);
}

 /* other Keywords */
 /* TODO: categorize this */
FOR|ENDFOR|CONTINUE|BREAK {
	printf("Token Type: ");
	printf("KEYWORD\n");
	printf("Value: %s\n", yytext);
}

 /* Operators */
 /* TODO: categorize this */
":="|"+"|"-"|"*"|"/"|"="|"!="|"<="|">="|"<"|">"|"("|")"|";"|"," {
	printf("Token Type: ");
	printf("OPERATOR\n");
	printf("Value: %s\n", yytext);
}

 /* IDENTIFIERS */
{ID} {
	printf("Token Type: ");
	printf("IDENTIFIER\n");
	printf("Value: %s\n", yytext);
}

 /* FLOATLITERAL */
-?{DIGIT}+"."{DIGIT}* {
	printf("Token Type: ");
	printf("FLOATLITERAL\n");
	printf("Value: %s\n", yytext);
}

 /* INTLITERAL */
 /* this is conflicting with "-" */
-?{DIGIT}+ {
	printf("Token Type: ");
	printf("INTLITERAL\n");
	printf("Value: %s\n", yytext);
}

 /* STRINGLITERAL */		
\"[^\"]*\" {
	printf("Token Type: ");
	printf("STRINGLITERAL\n");
	printf("Value: %s\n", yytext);
}

 /* COMMENT */ 
--[^\n]*\n {
}

 /* others */
.	{
	printf("Unrecognized character: %s\n", yytext);
	for(int i = 0;i < strlen(yytext);i++){
		printf("%d\n",(int)yytext[i]);
	}
}
%%

int main(int argc, char **argv){
	ifstream* fs = new ifstream();
	if (argc > 0){
		fs -> open(argv[1], ios::in);
	}
	FlexLexer* lexer = new yyFlexLexer(fs);
	lexer->yylex();

	return 0;
}
