#include "../src/ast.hpp"
#include <iostream>

using namespace std;

int temp_reg_index;
AddExprNode::AddExprNode(char sign){
    this->sign = sign;
    this->lnode = NULL;
    this->rnode = NULL;
}

AddExprNode::~AddExprNode(){}
string AddExprNode::translate(vector<string>& code_block){
    string op1 = lnode->translate(code_block);
    string op2 = rnode->translate(code_block);

    string new_ir = "";
    if(sign == '+') new_ir += "ADD";
    else new_ir += "SUB";

    if(type == "FLOAT") new_ir += "F ";
    else new_ir += "I ";

    new_ir += op1+" "+op2;
    string res = "$T"+to_string(temp_reg_index);
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
string MulExprNode::translate(vector<string>& code_block){
    string op1 = lnode->translate(code_block);
    string op2 = rnode->translate(code_block);

    string new_ir = "";
    if(sign == '*') new_ir += "MUL";
    else new_ir += "DIV";

    if(type == "FLOAT") new_ir += "F ";
    else new_ir += "I ";
    
    new_ir += op1+" "+op2;
    string res = "$T"+to_string(temp_reg_index);
    new_ir += " " + res;

    code_block.push_back(new_ir); // only integers for now, will modify later
    temp_reg_index++;

    return res;
}

// for now
CallExprNode::CallExprNode(string fname){

    this->name = fname;
    // this->arg_list = args;
}

CallExprNode::~CallExprNode(){}
string CallExprNode::translate(vector<string>& code_block){
    string res("not now...");
    return res;
}

CondExprNode::CondExprNode(string cmp):
    cmp(cmp)
{}

CondExprNode::~CondExprNode(){}

string CondExprNode::translate(vector<string>& code_block){

    cout << "BAKA!" << cmp << endl;
    string op1 = lnode->translate(code_block);
    string op2 = rnode->translate(code_block);

    //cmp op1 op2 label
    string new_ir = cmp + " " + op1 + " " + op2 + " LABEL_FOR_NOW";
    cout << op1 << " " << op2 << " " << new_ir << endl;
    code_block.push_back(new_ir);
    return cmp;
}


VarRef::VarRef(string name, string type){

    this->name = name;
    this->type = type;

    // this is a design flaw
    // VarRef should not have lnode/rnode
    // however for the sake of polymorphism...
    this->lnode = NULL;
    this->rnode = NULL;
}

VarRef::~VarRef(){}
string VarRef::translate(vector<string>& code_block){
    return name;
}  

LitRef::LitRef(string type, string val){

    this->type = type;
    this->value = val;

    // this is a design flaw
    // VarRef should not have lnode/rnode
    // however for the sake of polymorphism...
    this->lnode = NULL;
    this->rnode = NULL;
}

LitRef::~LitRef(){}
string LitRef::translate(vector<string>& code_block){

    /*
    string new_ir = "STORE";
    if(type == "FLOAT") new_ir += "F ";
    else new_ir += "I ";
    
    new_ir += value;
    string res = "$T"+to_string(temp_reg_index);
    new_ir += " "+res;

    code_block.push_back(new_ir); // only integers for now, will modify later
    temp_reg_index++;
    */

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

vector<string>& AssignStmtNode::translate(){
    
    update_AST_type(from); // for now
    vector<string>* code_block = new vector<string>;
    string res = from->translate(*code_block);
    string new_IR = "";
    if(to -> type == "INT") new_IR += "STOREI ";
    else if(to -> type == "FLOAT") new_IR += "STOREF ";
    
    new_IR += res + " " + to->name;
    code_block->push_back(new_IR);
    
    return *code_block;
}


ReadStmtNode::ReadStmtNode(){}
ReadStmtNode::~ReadStmtNode(){}
vector<string>& ReadStmtNode::translate(){
    vector<string>* code_block = new vector<string>;
    
    for(auto id:id_list){
        string new_IR = "";
        if(id -> type == "INT") new_IR += "READI ";
        else if(id -> type == "FLOAT") new_IR += "READF ";
        new_IR += id->name;
        code_block->push_back(new_IR);
    }
    
    return *code_block;
}


WriteStmtNode::WriteStmtNode(){}
WriteStmtNode::~WriteStmtNode(){}
vector<string>& WriteStmtNode::translate(){
    vector<string>* code_block = new vector<string>;
    
    for(auto id:id_list){
        string new_IR = "";
        if(id -> type == "INT") new_IR += "WRITEI ";
        else if(id -> type == "FLOAT") new_IR += "WRITEF ";
        else if(id -> type == "STRING") new_IR += "WRITES ";
        new_IR += id->name;
        code_block->push_back(new_IR);
    }
    
    return *code_block;
}

BlockNode::BlockNode(Symtable* symtable):
    symtable(symtable){}

BlockNode::~BlockNode(){}

FunctionDeclNode::FunctionDeclNode(string name, string type, 
        Symtable* symtable):
    BlockNode(symtable),
    name(name),type(type){}

FunctionDeclNode::~FunctionDeclNode(){}

vector<string>& FunctionDeclNode::translate(){
    
    cout << "FU" << endl;
    vector<string>* ir = new vector<string>;

    ir->push_back("LABEL FUNC_"+name);
    ir->push_back("LINK");

    for(auto stmt: stmt_list){
        vector<string> code_block = stmt->translate();
        ir->insert(ir->end(),code_block.begin(),code_block.end());
    }

    ir->push_back("RET"); //for now

    return *ir;
}

IfStmtNode::IfStmtNode(CondExprNode* cond, Symtable* symtable):
    BlockNode(symtable),
    cond(cond){}

IfStmtNode::~IfStmtNode(){}

vector<string>& IfStmtNode::translate(){
    cout << "CK" << endl;
    vector<string>* ir = new vector<string>;

    cout << "This is a if node" << endl;
    cond->translate(*ir);

    for(auto stmt: stmt_list){
        vector<string> code_block = stmt->translate();
        ir->insert(ir->end(),code_block.begin(),code_block.end());
    }

    cout << "translated..." << endl;
    return *ir;
}
