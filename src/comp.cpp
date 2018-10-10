#include <string>
#include <string.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stack>
#include "../src/symtable.hpp"
#include "../src/ast.hpp"
#include "../src/opt.hpp"
#include "../generated/parser.hpp"


extern FILE* yyin;
extern std::stack<Symtable*> symtable_stack; // should be size 1, with only the 
                                             // global symbol table left
extern std::vector<Symtable*> symtable_list; // this is redundant 
                                             // however needed in step3
extern std::vector<FunctionDeclNode*> func_list;


// translate $T** to r**,
// probably remove in the future 
// and re-implement translate()
std::string t2r(std::string& t){
    return "r" + t.substr(2);
}

std::vector<std::string>& ir2tiny(std::vector<std::vector<std::string>>& irs){

    std::vector<std::string>* tiny = new std::vector<std::string>;

    for(auto items:irs){
        if(items[0] == "LABEL"){
            // TODO: fill this in the following steps;
        }
        else if(items[0] == "LINK"){
            // TODO: fill this in the following steps;
        }
        else if(items[0] == "ADDI"){
            std::string target_reg = t2r(items[3]); 
            std::string op1;
            std::string op2;

            if(items[1][0] == '$') op1 = t2r(items[1]);
            else op1 = items[1];

            if(items[2][0] == '$') op2 = t2r(items[2]);
            else op2 = items[2];

            std::string new_tiny_op = "move " + op1 + " " + target_reg;
            tiny->push_back(new_tiny_op);
            new_tiny_op = "addi " + op2 + " " + target_reg;
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "SUBI"){
            std::string target_reg = t2r(items[3]); 
            std::string op1;
            std::string op2;

            if(items[1][0] == '$') op1 = t2r(items[1]);
            else op1 = items[1];

            if(items[2][0] == '$') op2 = t2r(items[2]);
            else op2 = items[2];

            std::string new_tiny_op = "move " + op1 + " " + target_reg;
            tiny->push_back(new_tiny_op);
            new_tiny_op = "subi " + op2 + " " + target_reg;
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "MULI"){
            std::string target_reg = t2r(items[3]); 
            std::string op1;
            std::string op2;

            if(items[1][0] == '$') op1 = t2r(items[1]);
            else op1 = items[1];

            if(items[2][0] == '$') op2 = t2r(items[2]);
            else op2 = items[2];

            std::string new_tiny_op = "move " + op1 + " " + target_reg;
            tiny->push_back(new_tiny_op);
            new_tiny_op = "muli " + op2 + " " + target_reg;
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "DIVI"){
            std::string target_reg = t2r(items[3]); 
            std::string op1;
            std::string op2;

            if(items[1][0] == '$') op1 = t2r(items[1]);
            else op1 = items[1];

            if(items[2][0] == '$') op2 = t2r(items[2]);
            else op2 = items[2];

            std::string new_tiny_op = "move " + op1 + " " + target_reg;
            tiny->push_back(new_tiny_op);
            new_tiny_op = "divi " + op2 + " " + target_reg;
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "ADDF"){
            std::string target_reg = t2r(items[3]); 
            std::string op1;
            std::string op2;

            if(items[1][0] == '$') op1 = t2r(items[1]);
            else op1 = items[1];

            if(items[2][0] == '$') op2 = t2r(items[2]);
            else op2 = items[2];

            std::string new_tiny_op = "move " + op1 + " " + target_reg;
            tiny->push_back(new_tiny_op);
            new_tiny_op = "addr " + op2 + " " + target_reg;
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "SUBF"){
            std::string target_reg = t2r(items[3]); 
            std::string op1;
            std::string op2;

            if(items[1][0] == '$') op1 = t2r(items[1]);
            else op1 = items[1];

            if(items[2][0] == '$') op2 = t2r(items[2]);
            else op2 = items[2];

            std::string new_tiny_op = "move " + op1 + " " + target_reg;
            tiny->push_back(new_tiny_op);
            new_tiny_op = "subr " + op2 + " " + target_reg;
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "MULF"){
            std::string target_reg = t2r(items[3]); 
            std::string op1;
            std::string op2;

            if(items[1][0] == '$') op1 = t2r(items[1]);
            else op1 = items[1];

            if(items[2][0] == '$') op2 = t2r(items[2]);
            else op2 = items[2];

            std::string new_tiny_op = "move " + op1 + " " + target_reg;
            tiny->push_back(new_tiny_op);
            new_tiny_op = "mulr " + op2 + " " + target_reg;
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "DIVF"){
            std::string target_reg = t2r(items[3]); 
            std::string op1;
            std::string op2;

            if(items[1][0] == '$') op1 = t2r(items[1]);
            else op1 = items[1];

            if(items[2][0] == '$') op2 = t2r(items[2]);
            else op2 = items[2];

            std::string new_tiny_op = "move " + op1 + " " + target_reg;
            tiny->push_back(new_tiny_op);
            new_tiny_op = "divr " + op2 + " " + target_reg;
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "STOREI"){
            std::string target_reg; 
            std::string op1;

            if(items[2][0] == '$') target_reg = t2r(items[2]);
            else target_reg = items[2];

            if(items[1][0] == '$') op1 = t2r(items[1]);
            else op1 = items[1];

            tiny->push_back("move " + op1 + " " + target_reg);
        }
        else if(items[0] == "STOREF"){
            std::string target_reg; 
            std::string op1;

            if(items[2][0] == '$') target_reg = t2r(items[2]);
            else target_reg = items[2];

            if(items[1][0] == '$') op1 = t2r(items[1]);
            else op1 = items[1];

            tiny->push_back("move " + op1 + " " + target_reg);
        }
        else if(items[0] == "READI"){
            std::string new_tiny_op = "sys readi ";
            new_tiny_op += items[1];
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "READF"){
            std::string new_tiny_op = "sys readr ";
            new_tiny_op += items[1];
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "WRITEI"){
            std::string new_tiny_op = "sys writei ";
            new_tiny_op += items[1];
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "WRITEF"){
            std::string new_tiny_op = "sys writer ";
            new_tiny_op += items[1];
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "WRITES"){
            std::string new_tiny_op = "sys writes ";
            new_tiny_op += items[1];
            tiny->push_back(new_tiny_op);
        }
    }

    return *tiny;
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

    std::vector<std::string>& ops = symtable_stack.top()->decl();
    for(auto func_node: func_list){
        std::vector<std::vector<std::string>>& ir = split_irs(func_node->translate());

        //live_ana(ir);
        std::vector<std::string>& op_decl = func_node->symtable->decl();
        std::vector<std::string>& op_block = ir2tiny(ir);
        ops.insert(ops.end(),op_decl.begin(),op_decl.end());
        ops.insert(ops.end(),op_block.begin(),op_block.end());
        //printing IR for debugging purpose
        /*
        for(auto line: ir){
            std::cout << line << std::endl;
        }
        std::cout << std::endl;
        */
    }

    for(auto op:ops){
        std::cout << op << std::endl;
    }

    for(auto table:symtable_list) delete table;
    fclose(fp);

    return 0;
}
