#include <string>
#include <string.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stack>
#include <iomanip>
#include "../inc/symtable/symtable.hpp"
#include "../inc/ast/expr/ExprNode.hpp"
#include "../inc/ast/stmt/StmtNode.hpp"
#include "../inc/ir/irNode.hpp"
#include "../inc/opt.hpp"
#include "../inc/utility.hpp"///////new add utility.cpp
#include "../generated/parser.hpp"

using namespace std;

extern FILE* yyin;
extern stack<Symtable*> symtable_stack; // should be size 1, with only the 
// global symbol table left
extern vector<FunctionDeclNode*> func_list;

int main(int argc, char** argv){

    if (argc != 2) {
        cerr << "Usage: ./compiler <source file>" << endl;
    }

    FILE* fp = fopen(argv[1],"r");
    yyin = fp;
    yyparse();

    fclose(fp);

    vector<string>& ops = symtable_stack.top()->decl();

    Symtable* globalSymtable = symtable_stack.top();
    FuncEntry* mainEntry = static_cast<FuncEntry*>(globalSymtable->have("main"));

    ops.push_back("push");          // push a space for return value of main
    for(int i = 0;i < mainEntry->getArgCnt(); i++) 
        ops.push_back("push");          // push a space for return value of main
    ops.push_back("push r0");          // push a space for return value of main
    ops.push_back("push r1");          // push a space for return value of main
    ops.push_back("push r2");          // push a space for return value of main
    ops.push_back("push r3");          // push a space for return value of main
    ops.push_back("jsr FUNC_main"); // call main
    ops.push_back("sys halt");      // end

    for(auto block_node: func_list){
        vector<IrNode*> irs = block_node->translate();
        // TODO: constant folding here
        IrNode::livenessAna();
        int linkIr = ops.size() + 1; // to find where is the link stmt

        for(auto ir: irs){
            /* printing debugging infos */
            cout << ";" ;
            ir->reformatPrint();
            /*--------------------------*/

            vector<string> codeBlock = ir->translate();

            /* printing debugging infos */
            cout << setfill(' ') << setw(20);
            if(!codeBlock.empty())  cout << codeBlock[0];
            else cout << " ";
            cout << ir->regMan.print().str() << endl;

            for(unsigned int i = 1; i < codeBlock.size(); i++) {
                cout << ';';
                cout << setfill(' ') << setw(100) << ' ';
                cout << codeBlock[i] << endl;
            }
            /*--------------------------*/
            ops.insert(ops.end(), codeBlock.begin(), codeBlock.end());
        }
        ops[linkIr] = "link " + to_string(block_node->getStackSize());
    }

    for(auto op:ops){
        cout << op << endl;
    }


    return 0;
}
