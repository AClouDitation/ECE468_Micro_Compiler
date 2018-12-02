#include "../../../inc/StmtNode.hpp"
#include "../../../inc/ExprNode.hpp"
#include "../../../inc/symtable.hpp"
#include "../../../inc/irNode.hpp"
#include "../../../inc/regman.hpp"
#include "../../../inc/utility.hpp"
#include <assert.h>

BlockNode::BlockNode(Symtable* symtable, FunctionDeclNode* farther):
    StmtNode(farther), symtable(symtable){}

BlockNode::~BlockNode(){}
