#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stack>
#include "../src/symtable.hpp"
#include "../src/ast.hpp"
#include "../generated/parser.hpp"


extern FILE* yyin;
extern std::stack<Symtable*> symtable_stack; // should be empty,
                                             // contents should be freed when they
                                             // were poped
extern std::vector<Symtable*> symtable_list; // this is redundant 
                                             // however needed in step3
extern std::vector<StmtNode*> stmt_list;    
int main(int argc, char** argv){
    
    FILE* fp = fopen(argv[1],"r");
    yyin = fp;
    yyparse();

    /* for step 3
    for(int i = 0;i < symtable_list.size();i++){
        symtable_list[i]->print();
        if(i != symtable_list.size()-1)std::cout << std::endl;
    }        
    */

    for(auto table:symtable_list){
        delete table;
    }

    for(auto stmt: stmt_list){
        std::vector<std::string> code_block = stmt->translate();
        for(auto line: code_block){
            std::cout << line << std::endl;
        }
    }



    fclose(fp);

    return 0;
}
