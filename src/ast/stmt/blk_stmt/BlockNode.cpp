#include "../../../../inc/ast/stmt/blk_stmt/BlockNode.hpp"
#include <assert.h>

BlockNode::BlockNode(Symtable* symtable, FunctionDeclNode* farther):
    StmtNode(farther), symtable(symtable){}

BlockNode::~BlockNode(){}
