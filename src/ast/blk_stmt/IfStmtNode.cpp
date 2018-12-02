#include "../../../inc/StmtNode.hpp"
#include "../../../inc/ExprNode.hpp"
#include "../../../inc/irNode.hpp"
#include "../../../inc/regman.hpp"
#include "../../../inc/utility.hpp"

using namespace std;

IfStmtNode::IfStmtNode(CondExprNode* cond, Symtable* symtable, string index,
        FunctionDeclNode* farther):
    BlockNode(symtable, farther),
    cond(cond),elseNode(NULL),index(index){}

IfStmtNode::~IfStmtNode(){}

vector<IrNode*>& IfStmtNode::translate(){
    vector<IrNode*>* ir = new vector<IrNode*>;
    cond->setOutLabel("ELSE_"+index);                           // give condition node label proper index
    cond->translate(*ir);                                       
    CondIrNode* condNode = static_cast<CondIrNode*>(ir->back());

    for(auto stmt: stmt_list){                                  // translate IF block
        vector<IrNode*> code_block = stmt->translate();
        irBlockCascade(*ir, code_block);
    }

    JumpIrNode* jmp = new JumpIrNode("END_IF_ELSE_"+index, *(farther->regMan));
    irBlockInsert(*ir, jmp);

    LabelIrNode* elseLabelNode = new LabelIrNode("ELSE_"+index, *(farther->regMan));
    irBlockInsert(*ir, elseLabelNode); 
    elseLabelNode->setPre(condNode);    // manually reset predecessor
    condNode->setSuc2(elseLabelNode);

    if(elseNode){   // translate else block
        vector<IrNode*> code_block = elseNode->translate();
        irBlockCascade(*ir, code_block);
    }
    
    LabelIrNode* endLabelNode = new LabelIrNode("END_IF_ELSE_"+index, *(farther->regMan));
    irBlockInsert(*ir, endLabelNode);
    endLabelNode->setPre2(jmp);
    jmp->setSuc(endLabelNode);

    return *ir;
}
