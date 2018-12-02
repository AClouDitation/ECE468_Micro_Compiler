#ifndef MULEXPRNODE_HPP_
#define MULEXPRNODE_HPP_

#include "ExprNode.hpp"
class MulExprNode: public ExprNode{
    char sign;
public:
    MulExprNode(FunctionDeclNode*, char);
    virtual ~MulExprNode();
    virtual std::string translate(std::vector<IrNode*>&);
};

#endif
