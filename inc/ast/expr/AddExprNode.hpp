#ifndef ADDEXPRNODE_HPP_
#define ADDEXPRNODE_HPP_

#include "ExprNode.hpp"

class AddExprNode: public ExprNode{
    char sign;
public:
    AddExprNode(FunctionDeclNode*, char);
    virtual ~AddExprNode();
    virtual std::string translate(std::vector<IrNode*>&);
};

#endif
