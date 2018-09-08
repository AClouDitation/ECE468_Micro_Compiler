#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include "../generated/parser.hpp"

extern FILE* yyin;
int main(int argc, char** argv){
    
    FILE* fp = fopen(argv[1],"r");
    yyin = fp;
    yyparse();

    std::cout<< "Accepted";

    return 0;
}
