#ifndef READSTMTNODE_HPP_
#define READSTMTNODE_HPP_

#include "BaseStmtNode.hpp"

class ReadStmtNode: public BaseStmtNode{
public:
    ReadStmtNode(FunctionDeclNode*);
    virtual ~ReadStmtNode();
    std::vector<IrNode*>& translate();
    std::vector<VarRef*> id_list;
};

#endif
