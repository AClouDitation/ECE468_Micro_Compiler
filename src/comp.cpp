#include <string>
#include <cstdlib>
#include "../generated/parser.hpp"
extern int yylex();
extern int yyparse();
extern FILE* yyin;

int main(int argc, char** argv){

    FILE* fp = fopen(argv[1],"r");
    yyin = fp;
    yyparse();

    return 0;
}
