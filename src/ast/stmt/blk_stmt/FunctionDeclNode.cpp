#include "../../../../inc/ast/stmt/blk_stmt/FunctionDeclNode.hpp"
#include "../../../../inc/ir/LabelIrNode.hpp"
#include "../../../../inc/ir/ReturnIrNode.hpp"
#include "../../../../inc/symtable/symtable.hpp"
#include "../../../../inc/regman/regman.hpp"
#include "../../../../inc/utility.hpp"
#include <assert.h>

using namespace std;

FunctionDeclNode::FunctionDeclNode
    (string name, string type, int argc, Symtable* symtable):
    BlockNode(symtable, NULL),name(name),type(type),argc(argc), 
    regMan(new regManager(4, this)){}

FunctionDeclNode::~FunctionDeclNode(){ delete regMan; }

vector<IrNode*>& FunctionDeclNode::translate(){
    
    vector<IrNode*>* ir = new vector<IrNode*>;

    irBlockInsert(*ir, new LabelIrNode("FUNC_"+name, *regMan));
    irBlockInsert(*ir, new IrNode("LINK", *regMan));        

    stackSize = symtable->size() - argc;

    for(auto stmt: stmt_list){
        vector<IrNode*> code_block = stmt->translate();     // translate one statment
        irBlockCascade(*ir, code_block);
    }

    // return if reach the end of function
    irBlockInsert(*ir, new ReturnIrNode(argc+6, *regMan));

    extern Symtable* globalSymtable;
    for(auto kv: globalSymtable->id_map) {
        if(kv.second->isFunc) continue;
        if(kv.second->type == "S") continue;
        ir->back()->insertOutSet(kv.first);
    }
    return *ir;
}

string FunctionDeclNode::getNextAvaTemp() { return "!T" + to_string(nextAvaTemp++); }   // get a new temporary

int FunctionDeclNode::getStackSize() { return stackSize; }
