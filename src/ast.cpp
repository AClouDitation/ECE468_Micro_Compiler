#include "ast.hpp"
#include <iostream>

using namespace std;
AddExprNode::AddExprNode(char type){
    
    cout << "new AddExprNode" << endl;  //debugging
    this->type = type;
    this->lnode = NULL;
    this->rnode = NULL;
}
AddExprNode::~AddExprNode(){}

MulExprNode::MulExprNode(char type){

    cout << "new MulExprNode" << endl;  //debugging
    this->type = type;
    this->lnode = NULL;
    this->rnode = NULL;
}
MulExprNode::~MulExprNode(){}

// for now
CallExprNode::CallExprNode(std::string fname){

    cout << "new CallExprNode" << endl;  //debugging
    this->name = fname;
    // this->arg_list = args;
}
CallExprNode::~CallExprNode(){}
VarRef::VarRef(std::string name, std::string type){

    cout << "new VarRef " << name << " " << type << endl;  //debugging
    this->name = name;
    this->type = type;

    // this is a design flaw
    // VarRef should not have lnode/rnode
    // however for the sake of polymorphism...
    this->lnode = NULL;
    this->rnode = NULL;
}
VarRef::~VarRef(){}
   
LitRef::LitRef(std::string type, std::string val){

    cout << "new LitRef " << val <<endl;  //debugging
    this->type = type;
    this->value = val;

    // this is a design flaw
    // VarRef should not have lnode/rnode
    // however for the sake of polymorphism...
    this->lnode = NULL;
    this->rnode = NULL;
}
LitRef::~LitRef(){}

AssignStmtNode::AssignStmtNode(){

    cout << "new AssignStmtNode" << endl;  //debugging
    this->to = NULL;
    this->from = NULL;
}

AssignStmtNode::~AssignStmtNode(){}
