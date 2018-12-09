#ifndef RETURNSTMTNODE_HPP_
#define RETURNSTMTNODE_HPP_

#include <string>
#include <vector>
#include "BaseStmtNode.hpp"

class ExprNode;
class VarRef;

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
