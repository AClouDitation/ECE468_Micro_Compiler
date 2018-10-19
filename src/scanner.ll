%{
    #include "../src/symtable.hpp"
    #include "../src/ast.hpp"
    #include "../generated/parser.hpp"
    extern char* yytext;
    #include <stdio.h>
    #include <stdlib.h>
    #include <string>
    #define YY_DECL int yylex()
%}

%option noyywrap nounput batch noinput
DIGIT [0-9]
IDENTIFIER 	    [_a-zA-Z][_a-zA-Z0-9]*	
INTLITERAL      {DIGIT}+
FLOATLITERAL    ({DIGIT}+"."{DIGIT}*)|({DIGIT}*"."{DIGIT}+)
STRINGLITERAL       \"[^\"]*\"
COMMENT             --.*
EATUPWHITESPACE     [ \t\r\n]+

%%
{EATUPWHITESPACE}   { ; }

{COMMENT}           { ; }

"PROGRAM"           { return TOK_PROGRAM; }
"BEGIN"             { return TOK_BEGIN; }
"END"               { return TOK_END; }
"FUNCTION"          { return TOK_FUNCTION; }
"READ"              { return TOK_READ; }
"WRITE"             { return TOK_WRITE; }
"IF"                { return TOK_IF; }
"ELSE"              { return TOK_ELSE; }
"ENDIF"             { return TOK_ENDIF; }
"WHILE"				{ return TOK_WHILE; }
"ENDWHILE"			{ return TOK_ENDWHILE; }
"RETURN"            { return TOK_RETURN; }
"INT"               { return TOK_INT; }
"VOID"              { return TOK_VOID; }
"STRING"            { return TOK_STRING; }
"FLOAT"             { return TOK_FLOAT; }
"TRUE"				{ return TOK_TRUE; }
"FALSE"				{ return TOK_FALSE; }

{IDENTIFIER}        { 
                        yylval.cstr = yytext;
                        return TOK_IDENTIFIER; 
                    }

{STRINGLITERAL}     { 
                        yylval.cstr = yytext;    
                        return TOK_STRINGLITERAL; 
                    }

{FLOATLITERAL}      {   
                        yylval.fval = atof(yytext);
                        return TOK_FLOATLITERAL;
                    }

{INTLITERAL}        {   
                        yylval.ival = atoi(yytext);
                        return TOK_INTLITERAL; 
                    }

":="                { return TOK_ASSIGN; }
"!="                {   
                        yylval.cstr = "NE"; 
                        return TOK_NEQ; 
                    }
"<="                { 
                        yylval.cstr = "LE";
                        return TOK_LEQ; 
                    }
">="                { 
                        yylval.cstr = "GE";
                        return TOK_GEQ; 
                    }
"+"	                { return TOK_PLUS; }
"-"                 { return TOK_MINUS; }
"*"                 { return TOK_MUL; }
"/"                 { return TOK_DIV; }
"="                 { return TOK_EQ; }
"<"                 { 
                        yylval.cstr = "LT"; 
                        return TOK_LT; 
                    }
">"                 { 
                        yylval.cstr = "GT"; 
                        return TOK_GT; 
                    }
"("                 { return TOK_OPAREN; }
")"                 { return TOK_CPAREN; }
";"                 { return TOK_SEMICOLON; }
","                 { return TOK_COMMA; }

%%

