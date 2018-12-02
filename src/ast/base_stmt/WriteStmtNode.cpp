#include "../../inc/ExprNode.hpp"
#include "../../inc/BaseStmtNode.hpp"
#include "../../inc/symtable.hpp"
#include "../../inc/irNode.hpp"
#include "../../inc/utility.hpp"

using namespace::std;

WriteStmtNode::WriteStmtNode(FunctionDeclNode* farther):
    BaseStmtNode(farther){}

WriteStmtNode::~WriteStmtNode(){} 

vector<IrNode*>& WriteStmtNode::translate(){
    vector<IrNode*>* code_block = new vector<IrNode*>;
    
    for(auto id:id_list){
        string type = id -> type;
        irBlockInsert(*code_block, new WriteIrNode(type, id->name, *(farther->regMan)));
    }
    
    return *code_block;
}