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
#include "../src/utility.hpp"///////new add utility.cpp

using namespace std;

extern FILE* yyin;
extern stack<Symtable*> symtable_stack; // should be size 1, with only the 
// global symbol table left
extern vector<Symtable*> symtable_list; // this is redundant 
// however needed in step3
extern vector<FunctionDeclNode*> func_list;

int main(int argc, char** argv){
    FILE* fp = fopen(argv[1],"r");
    yyin = fp;
    yyparse();

    extern int temp_reg_index;
    temp_reg_index = 0;
    vector<string>& ops = symtable_stack.top()->decl();
    for(auto block_node: func_list){
        vector<vector<string>>& ir = split_irs(block_node->translate());
        for(auto line: ir){
            for(auto item:line){
                cout << item << " ";
            }
            cout << endl;
        }
        cout << endl;

        //OOOptmize(ir);
        vector<string>& op_decl = block_node->symtable->decl();
        vector<string>& op_block = ir2tiny(ir);
        ops.insert(ops.end(),op_decl.begin(),op_decl.end());
        ops.insert(ops.end(),op_block.begin(),op_block.end());

        //printing IR for debugging purpose

        /*
           for(auto line: ir){
           cout << ";";
           for(auto item:line){
           cout << item << " ";
           }
           cout << endl;
           }
           cout << endl;
           */ 
    }

    for(auto op:ops){
        cout << op << endl;
    }

    for(auto table:symtable_list) delete table;
    fclose(fp);

    return 0;
}
