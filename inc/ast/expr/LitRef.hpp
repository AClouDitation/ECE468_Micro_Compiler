#ifndef LITREF_HPP_
#define LITREF_HPP_

#include "ExprNode.hpp"

class LitRef: public ExprNode{
public:
    LitRef(FunctionDeclNode*, std::string, std::string);
    virtual ~LitRef();
    virtual std::string translate(std::vector<IrNode*>&);

    std::string value;
};

#endif
