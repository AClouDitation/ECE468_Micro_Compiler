#include <string>
#include <string.h>
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
extern std::vector<FunctionDeclNode*> func_list;

std::vector<std::string> SplitString(const std::string& s, const std::string& c){

    std::vector<std::string> v;
    std::string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(std::string::npos != pos2){
        v.push_back(s.substr(pos1, pos2-pos1));

        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length()) v.push_back(s.substr(pos1));

    return v;
}

void ir2tiny(std::vector<std::string>& irs){
    for(auto ir:irs){
        std::vector<std::string> items = SplitString(ir, " ");
        if(items[0] == "LABEL"){
            // TODO: fill this in the following steps;
        }
        else if(items[0] == "LINK"){
            // TODO: fill this in the following steps;
        }
        else if(items[0] == "ADDI"){

        }
        else if(items[0] == "SUBI"){
        }
        else if(items[0] == "MULI"){
        }
        else if(items[0] == "DIVI"){
        }
        else if(items[0] == "ADDF"){
        }
        else if(items[0] == "SUBF"){
        }
        else if(items[0] == "MULF"){
        }
        else if(items[0] == "DIVF"){
        }
        else if(items[0] == "STOREI"){
        }
        else if(items[0] == "STOREF"){
        }
        else if(items[0] == "READI"){
        }
        else if(items[0] == "READF"){
        }
        else if(items[0] == "WRITEI"){
        }
        else if(items[0] == "WRITEF"){
        }
        else if(items[0] == "WRITES"){
        }
    }
}

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

    for(auto table:symtable_list) delete table;
    

    for(auto func_node: func_list){
        std::vector<std::string>& ir = func_node->translate();
        ir2tiny(ir);
        for(auto line: ir){
            std::cout << line << std::endl;
        }
        std::cout << std::endl;
    }
    //std::vector<std::string>& ir = AST2IR(); 



    fclose(fp);

    return 0;
}
