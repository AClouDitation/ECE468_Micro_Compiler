#include <string>
#include <string.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stack>
#include <iomanip>
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

    fclose(fp);

    vector<string>& ops = symtable_stack.top()->decl();

    Symtable* globalSymtable = symtable_stack.top();
    globalSymtable->print();
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
        IrNode::livenessAna();
        //printing debugging infos
        int lastIndex = ops.size();

        for(auto ir: irs){
            cout << ";" ;
            ir->reformatPrint();
            vector<string> codeBlock = ir->translate();
            cout << setfill(' ') << setw(20);
            if(!codeBlock.empty())  cout << codeBlock[0];
            else cout << " ";
            cout << ir->regMan.print().str() << endl;

            for(unsigned int i = 1; i < codeBlock.size(); i++) {
                cout << ';';
                cout << setfill(' ') << setw(100) << ' ';
                cout << codeBlock[i] << endl;
            }
            ops.insert(ops.end(), codeBlock.begin(), codeBlock.end());
        }
        ops[lastIndex+1] = "link " + to_string(block_node->getStackSize());
    }

    for(auto op:ops){
        cout << op << endl;
    }


    return 0;
}
