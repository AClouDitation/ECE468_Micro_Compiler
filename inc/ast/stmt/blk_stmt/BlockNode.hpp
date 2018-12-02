#ifndef BLOCKNODE_HPP_
#define BLOCKNODE_HPP_

#include "../StmtNode.hpp"

class FunctionDeclNode;
class BlockNode: public StmtNode{
public:
    BlockNode(Symtable*, FunctionDeclNode*);
    virtual ~BlockNode();
    virtual std::vector<IrNode*>& translate()=0;

    std::vector<StmtNode*> stmt_list;
    Symtable* symtable;
};

#endif
