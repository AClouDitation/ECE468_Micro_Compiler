#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stack>
#include "../src/symtable.hpp"
#include "../generated/parser.hpp"


extern FILE* yyin;
extern std::stack<Symtable*> symtable_list;
int main(int argc, char** argv){
    
    FILE* fp = fopen(argv[1],"r");
    yyin = fp;
    yyparse();

    std::cout<< "Accepted";
    while(symtable_list.size()){
        std::cout<<"BAKA!"<<std::endl;
        Symtable* curr = symtable_list.top();
        curr->print();
    }        

    return 0;
}
