#include "../../../inc/ast/expr/CallExprNode.hpp"
#include "../../../inc/ast/stmt/blk_stmt/FunctionDeclNode.hpp"
#include "../../../inc/ir/irNode.hpp"
#include "../../../inc/ir/PushIrNode.hpp"
#include "../../../inc/ir/PopIrNode.hpp"
#include "../../../inc/ir/CallIrNode.hpp"
#include "../../../inc/utility.hpp"
#include <assert.h>

using namespace std;

// for now
CallExprNode::CallExprNode(FunctionDeclNode* farther, string fname):
    ExprNode(farther),name(fname){}

CallExprNode::~CallExprNode(){}

string CallExprNode::translate(vector<IrNode*>& code_block){

    vector<string> args;
    int argc = 0;
    // prepare arguments
    while(!exprStack.empty()){
        argc++;
        string ret = exprStack.top()->translate(code_block);
        args.push_back(ret);
        exprStack.pop();
    }

    irBlockInsert(code_block, new PushIrNode(*(farther->regMan)));        // push a empty space to store return value of function
    irBlockInsert(code_block, new IrNode("PUSHREGS", *(farther->regMan))); // push all register inuse
    for(auto argToPush: args){                          // push arguments on stack
        irBlockInsert(code_block, new PushIrNode(argToPush, *(farther->regMan)));
    }
        
    irBlockInsert(code_block, new CallIrNode(name, *(farther->regMan)));    // call function

    for(int i = 0;i < argc;i++){                        // pop arguments
        irBlockInsert(code_block, new PopIrNode(*(farther->regMan)));
    }

    string res = farther->getNextAvaTemp();
    irBlockInsert(code_block, new IrNode("POPREGS", *(farther->regMan)));  // pop registers
    irBlockInsert(code_block, new PopIrNode(res, *(farther->regMan)));      // pop return value

    return res;
}
