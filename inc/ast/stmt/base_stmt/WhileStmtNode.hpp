#ifndef WHILESTMTNODE_HPP_
#define WHILESTMTNODE_HPP_

#include "BlockNode.hpp"

class WhileStmtNode: public BlockNode{
public:
    WhileStmtNode(CondExprNode*, Symtable*, std::string, FunctionDeclNode*);
    virtual ~WhileStmtNode();
    std::vector<IrNode*>& translate();

    CondExprNode* cond;
    std::string index;
};

#endif
