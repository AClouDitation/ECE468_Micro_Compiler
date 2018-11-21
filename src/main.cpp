#include <string>
#include <string.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stack>
#include "../inc/symtable.hpp"
#include "../inc/ExprNode.hpp"
#include "../inc/StmtNode.hpp"
#include "../inc/irNode.hpp"
#include "../inc/opt.hpp"
#include "../inc/utility.hpp"///////new add utility.cpp
#include "../generated/parser.hpp"

using namespace std;

extern FILE* yyin;
extern stack<Symtable*> symtable_stack; // should be size 1, with only the 
// global symbol table left
extern vector<FunctionDeclNode*> func_list;

int main(int argc, char** argv){
    FILE* fp = fopen(argv[1],"r");
    yyin = fp;
    yyparse();

    vector<string>& ops = symtable_stack.top()->decl();

    ops.push_back("push");          // push a space for return value of main
    ops.push_back("jsr FUNC_main"); // call main
    ops.push_back("sys halt");      // end

    for(auto block_node: func_list){
        //vector<vector<string>>& ir = split_irs(block_node->translate());
        //if (argc == 2 && strcmp(argv[1],"-o")) OOOptmize(ir);
        vector<IrNode*> irs = block_node->translate();
        
        //vector<string>& op_block = ir2tiny(ir);
        //ops.insert(ops.end(),op_block.begin(),op_block.end());

        //printing IR for debugging purpose

        for(auto ir: irs){
            cout << ";";
            ir->print();
            cout << " ";
            ir->printGen();
            cout << "\t\t";
            ir->printKill();
            cout << endl;
        }
        cout << endl;

    }

    /*
    for(auto op:ops){
        cout << op << endl;
    }
    */

    fclose(fp);

    return 0;
}
