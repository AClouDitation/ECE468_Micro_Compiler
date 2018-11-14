#include "StmtNode.hpp"
#include "ExprNode.hpp"
#include "../symtable/symtable.hpp"
#include <assert.h>

using namespace std;



BlockNode::BlockNode(Symtable* symtable):
    symtable(symtable){}

BlockNode::~BlockNode(){}

FunctionDeclNode::FunctionDeclNode
    (string name, string type, int argc, Symtable* symtable):
    BlockNode(symtable),name(name),type(type),argc(argc){}

FunctionDeclNode::~FunctionDeclNode(){}

vector<string>& FunctionDeclNode::translate(){
    
    vector<string>* ir = new vector<string>;

    ir->push_back("LABEL FUNC_"+name);
    ir->push_back("LINK " + to_string(symtable->size() - argc));

    for(auto stmt: stmt_list){
        vector<string> code_block = stmt->translate();
        ir->insert(ir->end(),code_block.begin(),code_block.end());
    }

    // return if reach the end of function
    ir->push_back("UNLINK");
    ir->push_back("RET");

    return *ir;
}

string FunctionDeclNode::getNextAvaTemp() {
    return "!T" + to_string(nextAvaTemp++);
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


