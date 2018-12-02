#include "../../../../inc/ast/stmt/base_stmt/AssignStmtNode.hpp"
#include "../../../../inc/ast/expr/VarRef.hpp"
#include "../../../../inc/ir/StoreIrNode.hpp"
#include "../../../../inc/symtable/symtable.hpp"
#include "../../../../inc/utility.hpp"

using namespace::std;

AssignStmtNode::AssignStmtNode(FunctionDeclNode* farther):
    BaseStmtNode(farther),to(NULL),from(NULL)
{}

AssignStmtNode::~AssignStmtNode(){}

void AssignStmtNode::update_AST_type(ExprNode* root){
    if(!root->lnode && !root->rnode) return; // this is a Ref Node
    if(root->lnode) update_AST_type(root->lnode);
    if(root->rnode) update_AST_type(root->rnode);

    if(root->lnode->type == "R" || root->rnode->type == "R"){
        root->type = "R"; 
    }
}

vector<IrNode*>& AssignStmtNode::translate(){
    
    update_AST_type(from); // for now
    vector<IrNode*>* code_block = new vector<IrNode*>;
    string res = from->translate(*code_block);
    string type = to->type;

    irBlockInsert(*code_block, new StoreIrNode(type, res, to->name, *(farther->regMan)));
    
    return *code_block;
}

