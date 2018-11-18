#include "../../inc/ExprNode.hpp"
#include "../../inc/BaseStmtNode.hpp"

using namespace::std;

BaseStmtNode::BaseStmtNode(FunctionDeclNode* farther):
    farther(farther)
{}

BaseStmtNode::~BaseStmtNode(){}

AssignStmtNode::AssignStmtNode(FunctionDeclNode* farther):
    BaseStmtNode(farther),to(NULL),from(NULL)
{}

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

    // this is because of the error in the tiny simulator
    // causing that in a move instruction, you cannot make
    // both operand memory refs
    if(from->is_var){
        string newReg = farther->getNextAvaTemp();
        code_block->push_back(new_IR + res + newReg);
        res = newReg;
    }

    new_IR += res + " " + to->name;
    code_block->push_back(new_IR);
    
    return *code_block;
}


ReadStmtNode::ReadStmtNode(FunctionDeclNode* farther):
    BaseStmtNode(farther){}

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


WriteStmtNode::WriteStmtNode(FunctionDeclNode* farther):
    BaseStmtNode(farther){}

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

ReturnStmtNode::ReturnStmtNode(FunctionDeclNode* farther, ExprNode* expr, int retLoc):
    BaseStmtNode(farther), expr(expr), retLoc(retLoc){}

ReturnStmtNode::~ReturnStmtNode(){}

vector<string>& ReturnStmtNode::translate(){
    vector<string>* ir = new vector<string>;
    string ret = expr->translate(*ir);
    string newReg = farther->getNextAvaTemp();

    ir->push_back("MOVE " + ret + " " + newReg);
    ir->push_back("MOVE " + newReg + " " + "$"+to_string(retLoc));
    ir->push_back("UNLINK");
    ir->push_back("RET");
    return *ir;
}
