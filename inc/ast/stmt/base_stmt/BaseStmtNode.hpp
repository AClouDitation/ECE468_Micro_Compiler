#ifndef BASESTMTNODE_HPP_
#define BASESTMTNODE_HPP_

#include <string>
#include <vector>
#include "../StmtNode.hpp"

class ExprNode;
class VarRef;

class BaseStmtNode: public StmtNode {
public:
    BaseStmtNode(FunctionDeclNode*);
    virtual ~BaseStmtNode();
    std::vector<IrNode*>& translate()=0;
};
