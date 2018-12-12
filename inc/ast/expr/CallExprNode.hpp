#ifndef CALLEXPRNODE_HPP_
#define CALLEXPRNODE_HPP_

#include <stack>
#include "ExprNode.hpp"

class CallExprNode: public ExprNode{
public:
    std::string name;
    CallExprNode(FunctionDeclNode*, std::string, std::string);
    virtual ~CallExprNode();
    virtual std::string translate(std::vector<IrNode*>&);

    std::stack<ExprNode*> exprStack;
};

#endif
