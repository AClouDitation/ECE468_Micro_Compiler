#include <string>
#include <cstdlib>
#include <fstream>
#include "../generated/parser.hpp"

extern FILE* yyin;
int main(int argc, char** argv){
    
    FILE* fp = fopen(argv[1],"r");
    yyin = fp;
    yyparse();

    return 0;
}
