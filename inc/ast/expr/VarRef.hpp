#ifndef VARREF_HPP_
#define VARREF_HPP_

#include "ExprNode.hpp"

class VarRef: public ExprNode{
public:
    VarRef(FunctionDeclNode*, std::string, std::string);
    virtual ~VarRef();
    virtual std::string translate(std::vector<IrNode*>&);
    
    std::string name;
};

#endif
