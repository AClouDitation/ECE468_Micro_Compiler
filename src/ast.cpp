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

void AssignStmtNode::update_AST_type(ExprNode* root){
    if(!root->lnode && !root->rnode) return; // this is a Ref Node
    if(root->lnode) update_AST_type(root->lnode);
    if(root->rnode) update_AST_type(root->rnode);

    if(root->lnode->type == "FLOAT" || root->rnode->type == "FLOAT"){
        root->type = "FLOAT"; 
    }
}

std::vector<std::string>& AssignStmtNode::translate(){
    
    update_AST_type(from); // for now
    std::vector<std::string>* code_block = new std::vector<std::string>;
    std::string res = from->translate(*code_block);
    std::string new_IR = "";
    if(to -> type == "INT") new_IR += "STOREI ";
    else if(to -> type == "FLOAT") new_IR += "STOREF ";
    
    new_IR += res + " " + to->name;
    code_block->push_back(new_IR);
    
    return *code_block;
}


ReadStmtNode::ReadStmtNode(){}
ReadStmtNode::~ReadStmtNode(){}
std::vector<std::string>& ReadStmtNode::translate(){
    std::vector<std::string>* code_block = new std::vector<std::string>;
    
    for(auto id:id_list){
        std::string new_IR = "";
        if(id -> type == "INT") new_IR += "READI ";
        else if(id -> type == "FLOAT") new_IR += "READF ";
        new_IR += id->name;
        code_block->push_back(new_IR);
    }
    

    return *code_block;
}


WriteStmtNode::WriteStmtNode(){}
WriteStmtNode::~WriteStmtNode(){}
std::vector<std::string>& WriteStmtNode::translate(){
    std::vector<std::string>* code_block = new std::vector<std::string>;

    return *code_block;
}

FunctionDeclNode::FunctionDeclNode(std::string name, std::string type):
    name(name),type(type){}

FunctionDeclNode::~FunctionDeclNode(){}



std::vector<std::string>& FunctionDeclNode::translate(){
    
    std::vector<std::string>* ir = new std::vector<std::string>;

    ir->push_back("LABEL FUNC_"+name);
    ir->push_back("LINK");

    for(auto stmt: stmt_list){
        std::vector<std::string> code_block = stmt->translate();
        ir->insert(ir->end(),code_block.begin(),code_block.end());
    }

    ir->push_back("RET"); //for now

    return *ir;
}
