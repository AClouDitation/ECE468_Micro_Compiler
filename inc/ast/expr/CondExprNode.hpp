#ifndef CONDEXPRNODE_HPP_
#define CONDEXPRNODE_HPP_

#include "ExprNode.hpp"

class CondExprNode: public ExprNode{
    std::string out_label;
public:
    CondExprNode(FunctionDeclNode*, std::string);
    virtual ~CondExprNode();    
    void setOutLabel(std::string);
    virtual std::string translate(std::vector<IrNode*>&);

    std::string cmp;
};

#endif
