#include "../../../../inc/ast/expr/ExprNode.hpp"
#include "../../../../inc/ast/stmt/base_stmt/ReturnStmtNode.hpp"
#include "../../../../inc/symtable/symtable.hpp"
#include "../../../../inc/ir/StoreIrNode.hpp"
#include "../../../../inc/ir/ReturnIrNode.hpp"
#include "../../../../inc/utility.hpp"

using namespace::std;

ReturnStmtNode::ReturnStmtNode(FunctionDeclNode* farther, ExprNode* expr, int retLoc):
    BaseStmtNode(farther), expr(expr), retLoc(retLoc){}

ReturnStmtNode::~ReturnStmtNode(){}

vector<IrNode*>& ReturnStmtNode::translate(){
    vector<IrNode*>* ir = new vector<IrNode*>;
    string ret = expr->translate(*ir);
    string type = expr->type.substr(0,1);
    string newReg = farther->getNextAvaTemp();

    irBlockInsert(*ir, new StoreIrNode(type, ret, newReg, *(farther->regMan)));
    irBlockInsert(*ir, new StoreIrNode(type, newReg,"$"+to_string(retLoc), *(farther->regMan)));
    irBlockInsert(*ir, new ReturnIrNode(retLoc, *(farther->regMan)));
    
    // mark all global var as live here
    extern Symtable* globalSymtable;
    for(auto kv: globalSymtable->id_map) {
        if(kv.second->isFunc) continue;
        if(kv.second->type == "S") continue;
        ir->back()->insertOutSet(kv.first);
    }

    return *ir;
}
