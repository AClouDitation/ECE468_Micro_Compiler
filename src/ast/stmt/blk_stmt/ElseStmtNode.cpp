#include "../../../../inc/ast/stmt/blk_stmt/ElseStmtNode.hpp"
#include "../../../../inc/utility.hpp"

using namespace std;

ElseStmtNode::ElseStmtNode(Symtable* symtable, FunctionDeclNode* farther):
    BlockNode(symtable, farther){}

ElseStmtNode::~ElseStmtNode(){}

vector<IrNode*>& ElseStmtNode::translate(){
    vector<IrNode*>* ir = new vector<IrNode*>;

    for(auto stmt: stmt_list){
        vector<IrNode*> code_block = stmt->translate();
        irBlockCascade(*ir, code_block);
    }

    return *ir;
}
