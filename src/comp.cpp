#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stack>
#include "../src/symtable.hpp"
#include "../generated/parser.hpp"


extern FILE* yyin;
extern std::stack<Symtable*> symtable_stack;
extern std::vector<Symtable*> symtable_list;
int main(int argc, char** argv){
    
    FILE* fp = fopen(argv[1],"r");
    yyin = fp;
    yyparse();

    //std::cout<< "Accepted" << std::endl << std::endl;
    for(auto it :symtable_list){
        it->print();
        std::cout << std::endl;
    }        

    return 0;
}
