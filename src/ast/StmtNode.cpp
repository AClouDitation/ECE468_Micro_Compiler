#include "../../inc/StmtNode.hpp"
#include "../../inc/ExprNode.hpp"
#include "../../inc/symtable.hpp"
#include "../../inc/irNode.hpp"
#include "../../inc/regman.hpp"
#include "../../inc/utility.hpp"
#include <assert.h>

using namespace std;

StmtNode::StmtNode(FunctionDeclNode* farther): farther(farther) {}
StmtNode::~StmtNode() {}

BlockNode::BlockNode(Symtable* symtable, FunctionDeclNode* farther):
    StmtNode(farther), symtable(symtable){}

BlockNode::~BlockNode(){}

FunctionDeclNode::FunctionDeclNode
    (string name, string type, int argc, Symtable* symtable):
    BlockNode(symtable, NULL),name(name),type(type),argc(argc), 
    regMan(new regManager(4, this)){}

FunctionDeclNode::~FunctionDeclNode(){
    delete regMan;
}

vector<IrNode*>& FunctionDeclNode::translate(){
    
    vector<IrNode*>* ir = new vector<IrNode*>;

    irBlockInsert(*ir, new LabelIrNode("FUNC_"+name, *regMan));
    irBlockInsert(*ir, new IrNode("LINK", *regMan));        

    stackSize = symtable->size() - argc;

    for(auto stmt: stmt_list){
        vector<IrNode*> code_block = stmt->translate();                         // translate one statment
        irBlockCascade(*ir, code_block);
    }

    // return if reach the end of function
    irBlockInsert(*ir, new IrNode("UNLNK", *regMan));
    irBlockInsert(*ir, new ReturnIrNode(argc+6, *regMan));

    // TODO: make a class for return node
    // and puth the following part into it
    // mark all global var as live here
    extern Symtable* globalSymtable;
    for(auto kv: globalSymtable->id_map) {
        if(kv.second->isFunc) continue;
        if(kv.second->type == "S") continue;
        ir->back()->insertOutSet(kv.first);
    }
    return *ir;
}

string FunctionDeclNode::getNextAvaTemp() {
    return "!T" + to_string(nextAvaTemp++);                             // get a new temporary
}

int FunctionDeclNode::getStackSize() { return stackSize;}

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
    endLabelNode->setPre2(condNode);
    condNode->setSuc2(endLabelNode);
    return *ir;
}


