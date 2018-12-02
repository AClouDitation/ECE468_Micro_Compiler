#ifndef ASSIGNSTMTNODE_HPP_
#define ASSIGNSTMTNODE_HPP_

#include <string>
#include <vector>
#include "BaseStmtNode.hpp"

class ExprNode;
class VarRef;


class AssignStmtNode: public BaseStmtNode{
public:
    AssignStmtNode(FunctionDeclNode*);
    virtual ~AssignStmtNode();

    void update_AST_type(ExprNode*);
    std::vector<IrNode*>& translate();
    
    VarRef* to;
    ExprNode* from;
};

#endif