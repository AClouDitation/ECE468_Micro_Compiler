#include "../../../inc/StmtNode.hpp"
#include "../../../inc/ExprNode.hpp"
#include "../../../inc/irNode.hpp"
#include "../../../inc/regman.hpp"
#include "../../../inc/utility.hpp"

using namespace std;

WhileStmtNode::WhileStmtNode(CondExprNode* cond, Symtable* symtable, string index,
        FunctionDeclNode* farther):
    BlockNode(symtable, farther),
    cond(cond),index(index){}

WhileStmtNode::~WhileStmtNode(){}

vector<IrNode*>& WhileStmtNode::translate(){
    vector<IrNode*>* ir = new vector<IrNode*>;

    LabelIrNode* begin = new  LabelIrNode("WHILE_START_"+index, *(farther->regMan));     // save the begin node of the loop
    irBlockInsert(*ir, begin);
    cond->setOutLabel("END_WHILE_"+index);
    cond->translate(*ir);
    CondIrNode* condNode = static_cast<CondIrNode*>(ir->back());  // save the condNode

    for(auto stmt: stmt_list){
        vector<IrNode*> code_block = stmt->translate();
        irBlockCascade(*ir, code_block);
    }
    
    JumpIrNode* jmp = new JumpIrNode("WHILE_START_"+index, *(farther->regMan)); 
    irBlockInsert(*ir, jmp);
    begin->setPre2(jmp);    // link the begin label node with jump node 
                            // jmp label ----> label

    LabelIrNode* endLabelNode = new LabelIrNode("END_WHILE_"+index, *(farther->regMan));
    irBlockInsert(*ir, endLabelNode);
    jmp->setSuc(begin);
    endLabelNode->setPre(condNode);
    condNode->setSuc2(endLabelNode);
    return *ir;
}

