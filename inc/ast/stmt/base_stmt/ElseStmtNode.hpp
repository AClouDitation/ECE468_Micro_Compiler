#ifndef ELSESTMTNODE_HPP_
#define ELSESTMTNODE_HPP_

#include "BlockNode.hpp"

class ElseStmtNode: public BlockNode{
public:
    ElseStmtNode(Symtable*, FunctionDeclNode*);
    virtual ~ElseStmtNode();
    virtual std::vector<IrNode*>& translate();

};
#endif
