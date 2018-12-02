#include "../../inc/ExprNode.hpp"
#include "../../inc/BaseStmtNode.hpp"
#include "../../inc/symtable.hpp"
#include "../../inc/irNode.hpp"
#include "../../inc/utility.hpp"

using namespace::std;

BaseStmtNode::BaseStmtNode(FunctionDeclNode* farther):
    StmtNode(farther)
{}

BaseStmtNode::~BaseStmtNode(){}



