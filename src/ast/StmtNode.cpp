#include "../../inc/StmtNode.hpp"
#include "../../inc/ExprNode.hpp"
#include "../../inc/symtable.hpp"
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
    cond->setOutLabel("ELSE_"+index);
    cond->translate(*ir);

    for(auto stmt: stmt_list){  // translate if block
        vector<string> code_block = stmt->translate();
        ir->insert(ir->end(),code_block.begin(),code_block.end());
    }
    ir->push_back("JUMP END_IF_ELSE_"+index);
    ir->push_back("LABEL ELSE_"+index);
    if(elseNode){               // translate else block
        vector<string> code_block = elseNode->translate();
        ir->insert(ir->end(),code_block.begin(),code_block.end());
    }
    else {} // TODO: if there is no else node, set the condition jump to out directly
    
    ir->push_back("LABEL END_IF_ELSE_"+index);
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
    cond->setOutLabel("END_WHILE_"+index);
    cond->translate(*ir);

    for(auto stmt: stmt_list){
        vector<string> code_block = stmt->translate();
        ir->insert(ir->end(),code_block.begin(),code_block.end());
    }
    
    ir->push_back("JUMP WHILE_START_"+index);
    ir->push_back("LABEL END_WHILE_"+index);
    return *ir;

}


