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
std::string AddExprNode::translate(std::vector<std::string>& code_block){
    std::string op1 = lnode->translate(code_block);
    std::string op2 = rnode->translate(code_block);

    std::string new_ir = "";
    if(type == '+') new_ir += "ADDI ";
    else new_ir += "SUBI ";

    new_ir += op1+" "+op2;
    std::string res = "$T"+std::to_string(temp_reg_index);
    new_ir += " " + res;

    code_block.push_back(new_ir); // only integers for now, will modify later
    temp_reg_index++;

    return res;
}

MulExprNode::MulExprNode(char type){

    cout << "new MulExprNode" << endl;  //debugging
    this->type = type;
    this->lnode = NULL;
    this->rnode = NULL;
}
MulExprNode::~MulExprNode(){}
std::string MulExprNode::translate(std::vector<std::string>& code_block){
    std::string op1 = lnode->translate(code_block);
    std::string op2 = rnode->translate(code_block);

    std::string new_ir = "";
    if(type == '*') new_ir += "MULI ";
    else new_ir += "DIVI ";

    new_ir += op1+" "+op2;
    std::string res = "$T"+std::to_string(temp_reg_index);
    new_ir += " " + res;

    code_block.push_back(new_ir); // only integers for now, will modify later
    temp_reg_index++;

    return res;
}

// for now
CallExprNode::CallExprNode(std::string fname){

    cout << "new CallExprNode" << endl;  //debugging
    this->name = fname;
    // this->arg_list = args;
}
CallExprNode::~CallExprNode(){}
std::string CallExprNode::translate(std::vector<std::string>& code_block){
    std::string res("not now...");
    return res;
}


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
std::string VarRef::translate(std::vector<std::string>& code_block){
    return name;
}  

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
std::string LitRef::translate(std::vector<std::string>& code_block){
    return value;
}

AssignStmtNode::AssignStmtNode(){

    cout << "new AssignStmtNode" << endl;  //debugging
    this->to = NULL;
    this->from = NULL;
}

AssignStmtNode::~AssignStmtNode(){}
std::vector<std::string>& AssignStmtNode::translate(){
    
    vector<std::string>* code_block = new vector<std::string>;
    std::string res = from->translate(*code_block);
    std::string new_IR = "";
    if(to -> type == "INT") new_IR += "STOREI ";
    else if(to -> type == "FLOAT") new_IR += "STOREF ";
    
    new_IR += res + " " + to->name;
    code_block->push_back(new_IR);
    
    return *code_block;
}
