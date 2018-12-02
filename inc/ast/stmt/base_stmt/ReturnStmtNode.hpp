#ifndef RETURNSTMTNODE_HPP_
#define RETURNSTMTNODE_HPP_

#include <string>
#include <vector>
#include "BaseStmtNode.hpp"

class ExprNode;
class VarRef;

// control stmt
// TODO: separate control statment nodes
// TODO: add break/continue
class ReturnStmtNode: public BaseStmtNode{
    ExprNode* expr;
    int retLoc;
public:
    ReturnStmtNode(FunctionDeclNode*, ExprNode*, int);
    virtual ~ReturnStmtNode();
    virtual std::vector<IrNode*>& translate();
};

#endif