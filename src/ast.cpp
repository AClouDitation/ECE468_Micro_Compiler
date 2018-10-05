#include <ast.hpp>

AddExprNode::AddExprNode(char type){
    this->type = type;
    this->left = NULL;
    this->right = NULL;
}
AddExprNode::~AddExprNode(){}

MulExprNode::MulExprNode(char type){
    this->type = type;
    this->left = NULL;
    this->right = NULL;
}
MulExprNode::~MulExprNode(){}

VarRef::VarRef(string name, string type){
    this->name = name;
    this->type = type;
}
VarRef::~VarRef(){}
   
AssignStmtNode::AssignStmtNode(){
    this->to = NULL;
    this->from = NULL;
}

AssignStmtNode::~AssignStmtNode(){}
