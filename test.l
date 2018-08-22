%{
#include <math.h>
#include <cstdio>
#include <iostream>
#include <fstream>
using namespace std;
%}
%option noyywrap
DIGIT [0-9]
ID 	[a-zA-Z][a-zA-Z0-9]*	

%%
 /* INTLITERAL */

 /* IDENTIFIERS */
{ID} {
	printf("An idenifier: %s\n", yytext);
}

 /* INTLITERAL */
{DIGIT}+ {
		printf("An integer: %s (%d)\n",yytext, atoi(yytext));
		}

 /* FLOATLITERAL */
{DIGIT}+"."{DIGIT}* {
		printf("An float: %s (%g)\n", yytext, atof(yytext));
		}

 /* STRINGLITERAL */		
\"[^\"]\" {
		printf("A string: %s\n", yytext);
}

 /* COMMENT */ 
--[\s]*\n

 /* Keywords */
PROGRAM|BEGIN|END|FUNCTION|READ|WRITE|IF|ELSE|ENDIF|WHILE|ENDWHILE|RETURN|INT|VOID|STRING|FLOAT|TRUE|FALSE {
	printf("A Keyword: %s\n", yytext);
}
 /* other Keywords */
FOR|ENDFOR|CONTINUE|BREAK {
	printf("A Keyword: %s\n", yytext);
}

 /* Operators */
":="|"+"|"-"|"*"|"/"|"="|"!="|"<"|">"|"("|")"|";"|","|"<="|">=" {
	printf("An operator: %s\n", yytext);
}
 /* rm whitespace */
[ \t\n]+	

 /* others */
.	printf("Unrecognized character: %s\n", yytext);

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
