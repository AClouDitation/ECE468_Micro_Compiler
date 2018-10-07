#include "ast.hpp"
#include <iostream>

AddExprNode::AddExprNode(char sign){
    this->sign = sign;
    this->lnode = NULL;
    this->rnode = NULL;
}

AddExprNode::~AddExprNode(){}
std::string AddExprNode::translate(std::vector<std::string>& code_block){
    std::string op1 = lnode->translate(code_block);
    std::string op2 = rnode->translate(code_block);

    std::string new_ir = "";
    if(sign == '+') new_ir += "ADD";
    else new_ir += "SUB";

    if(type == "FLOAT") new_ir += "F ";
    else new_ir += "I ";

    new_ir += op1+" "+op2;
    std::string res = "$T"+std::to_string(temp_reg_index);
    new_ir += " " + res;

    code_block.push_back(new_ir); // only integers for now, will modify later
    temp_reg_index++;

    return res;
}

MulExprNode::MulExprNode(char sign){
    this->sign = sign;
    this->lnode = NULL;
    this->rnode = NULL;
}
MulExprNode::~MulExprNode(){}
std::string MulExprNode::translate(std::vector<std::string>& code_block){
    std::string op1 = lnode->translate(code_block);
    std::string op2 = rnode->translate(code_block);

    std::string new_ir = "";
    if(sign == '*') new_ir += "MUL";
    else new_ir += "DIV";

    if(type == "FLOAT") new_ir += "F ";
    else new_ir += "I ";
    
    new_ir += op1+" "+op2;
    std::string res = "$T"+std::to_string(temp_reg_index);
    new_ir += " " + res;

    code_block.push_back(new_ir); // only integers for now, will modify later
    temp_reg_index++;

    return res;
}

// for now
CallExprNode::CallExprNode(std::string fname){

    this->name = fname;
    // this->arg_list = args;
}
CallExprNode::~CallExprNode(){}
std::string CallExprNode::translate(std::vector<std::string>& code_block){
    std::string res("not now...");
    return res;
}


VarRef::VarRef(std::string name, std::string type){

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

    this->to = NULL;
    this->from = NULL;
}

AssignStmtNode::~AssignStmtNode(){}
std::vector<std::string>& AssignStmtNode::translate(){
    
    std::vector<std::string>* code_block = new std::vector<std::string>;
    std::string res = from->translate(*code_block);
    std::string new_IR = "";
    if(to -> type == "INT") new_IR += "STOREI ";
    else if(to -> type == "FLOAT") new_IR += "STOREF ";
    
    new_IR += res + " " + to->name;
    code_block->push_back(new_IR);
    
    return *code_block;
}
