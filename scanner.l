%{
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstring>
#include "compiler_driver.hpp"
%}
%option noyywrap
DIGIT [0-9]
ID 	[_a-zA-Z][_a-zA-Z0-9]*	

%%

 /* rm whitespace */
[ \r\n\t]+ {
}

 /* Keywords */
 /* Segment Keywords */
PROGRAM|BEGIN|END|FUNCTION {
	printf("Token Type: ");
	printf("KEYWORD\n");
	printf("Value: %s\n", yytext);
}

 /* R/W Keywords */
READ|WRITE {
	printf("Token Type: ");
	printf("KEYWORD\n");
	printf("Value: %s\n", yytext);
}

 /* Branching Keywords */
IF|ELSE|ENDIF {
	printf("Token Type: ");
	printf("KEYWORD\n");
	printf("Value: %s\n", yytext);
}
 /* Bool Value Keywords */
TRUE|FALSE {
	printf("Token Type: ");
	printf("KEYWORD\n");
	printf("Value: %s\n", yytext);
}

 /* Datatype Keywords */
INT|VOID|STRING|FLOAT {
	printf("Token Type: ");
	printf("KEYWORD\n");
	printf("Value: %s\n", yytext);
}

 /* Control Keywords */
CONTINUE|BREAK|RETURN {
	printf("Token Type: ");
	printf("KEYWORD\n");
	printf("Value: %s\n", yytext);
}

 /* Loop Keywords */
FOR|ENDFOR|WHILE|ENDWHILE {
	printf("Token Type: ");
	printf("KEYWORD\n");
	printf("Value: %s\n", yytext);
}

 /* Operators */
 /* Value Assign Operators */
":=" {
	printf("Token Type: ");
	printf("OPERATOR\n");
	printf("Value: %s\n", yytext);
}

 /* Arithmetic Operators */
"+"|"-"|"*"|"/" {
	printf("Token Type: ");
	printf("OPERATOR\n");
	printf("Value: %s\n", yytext);
}

 /* Comparision Operators */
"="|"!="|"<="|">="|"<"|">" {
	printf("Token Type: ");
	printf("OPERATOR\n");
	printf("Value: %s\n", yytext);
}

 /* MISC Operators */ 
 "("|")"|";"|"," {
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
{DIGIT}*"."{DIGIT}* {
	printf("Token Type: ");
	printf("FLOATLITERAL\n");
	printf("Value: %s\n", yytext);
}

 /* INTLITERAL */
{DIGIT}+ {
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

 /* Others, Raise error message*/
.	{
	printf("Unrecognized character: %s\n", yytext);
}
%%

void compiler_driver::scan_begin(){
	lexer = new yyFlexLexer(fs);
	lexer->yylex();
}

void compiler_driver::scan_end(){
	delete lexer;
}
