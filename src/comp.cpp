#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stack>
#include "../src/symtable.hpp"
#include "../src/ast.hpp"
#include "../generated/parser.hpp"


extern FILE* yyin;
extern std::stack<Symtable*> symtable_stack;
extern std::vector<Symtable*> symtable_list;
int main(int argc, char** argv){
    
    FILE* fp = fopen(argv[1],"r");
    yyin = fp;
    yyparse();

    //std::cout<< "Accepted" << std::endl << std::endl;
    for(int i = 0;i < symtable_list.size();i++){
        symtable_list[i]->print();
        if(i != symtable_list.size()-1)std::cout << std::endl;
    }        

    for(auto table:symtable_list){
        delete table;
    }

    fclose(fp);

    return 0;
}
