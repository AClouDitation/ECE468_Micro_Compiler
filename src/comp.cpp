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

// translate $T** to r**,
// probably remove in the future 
// and re-implement translate()
std::string t2r(std::string& t){
    return "r" + t.substr(2);
}

std::vector<std::string>& ir2tiny(std::vector<std::string>& irs){

    std::vector<std::string>* tiny = new std::vector<std::string>;

    for(auto ir:irs){
        std::vector<std::string> items = SplitString(ir, " ");
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
            new_tiny_op = "addf " + op2 + " " + target_reg;
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
            new_tiny_op = "subf " + op2 + " " + target_reg;
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
            new_tiny_op = "mulf " + op2 + " " + target_reg;
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
            new_tiny_op = "divf " + op2 + " " + target_reg;
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
            std::string new_tiny_op = "sys readf ";
            new_tiny_op += items[1];
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "WRITEI"){
            std::string new_tiny_op = "sys writei ";
            new_tiny_op += items[1];
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "WRITEF"){
            std::string new_tiny_op = "sys writef ";
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

    for(auto table:symtable_list) delete table;
    

    std::vector<std::string> ops;
    for(auto func_node: func_list){
        std::vector<std::string>& ir = func_node->translate();
        std::vector<std::string>& op_block = ir2tiny(ir);
        ops.insert(ops.end(),op_block.begin(),op_block.end());
        for(auto line: ir){
            std::cout << line << std::endl;
        }
        std::cout << std::endl;
    }

    for(auto op:ops){
        std::cout << op << std::endl;
    }



    fclose(fp);

    return 0;
}
