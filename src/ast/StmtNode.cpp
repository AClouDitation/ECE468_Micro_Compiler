#include "StmtNode.hpp"

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

    // this is because of the error in the tiny simulator
    // causing that in a move instruction, you cannot make
    // both operand memory refs
    
    extern int temp_reg_index;
    if(from->is_var){
        code_block->push_back(new_IR + res + " $T" + to_string(temp_reg_index));
        res = "$T"+to_string(temp_reg_index++);
    }

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

IfStmtNode::IfStmtNode(CondExprNode* cond, Symtable* symtable, string index):
    BlockNode(symtable),
    cond(cond),elseNode(NULL),index(index){}

IfStmtNode::~IfStmtNode(){}

vector<string>& IfStmtNode::translate(){
    vector<string>* ir = new vector<string>;

    cond->translate(*ir);
    ir->back() += index;

    if(elseNode){
        vector<string> code_block = elseNode->translate();
        ir->insert(ir->end(),code_block.begin(),code_block.end());
    }


    ir->push_back("JUMP OUT_"+index);
    ir->push_back("LABEL SUCCESS_"+index);
    for(auto stmt: stmt_list){
        vector<string> code_block = stmt->translate();
        ir->insert(ir->end(),code_block.begin(),code_block.end());
    }

    
    ir->push_back("LABEL OUT_"+index);
    return *ir;
}

ElseStmtNode::ElseStmtNode(Symtable* symtable):
    BlockNode(symtable)
{}

ElseStmtNode::~ElseStmtNode(){}

vector<string>& ElseStmtNode::translate(){
    vector<string>* ir = new vector<string>;

    for(auto stmt: stmt_list){
        vector<string> code_block = stmt->translate();
        ir->insert(ir->end(),code_block.begin(),code_block.end());
    }

    return *ir;
}

WhileStmtNode::WhileStmtNode(CondExprNode* cond, Symtable* symtable, string index):
    BlockNode(symtable),
    cond(cond),index(index){}

WhileStmtNode::~WhileStmtNode(){}

vector<string>& WhileStmtNode::translate(){
    vector<string>* ir = new vector<string>;

    ir->push_back("LABEL WHILE_START_"+index);
    cond->translate(*ir);
    ir->back() += index;

    ir->push_back("JUMP OUT_"+index);
    ir->push_back("LABEL SUCCESS_"+index);
    for(auto stmt: stmt_list){
        vector<string> code_block = stmt->translate();
        ir->insert(ir->end(),code_block.begin(),code_block.end());
    }

    ir->push_back("JUMP WHILE_START_"+index);
    
    ir->push_back("LABEL OUT_"+index);
    return *ir;

}
