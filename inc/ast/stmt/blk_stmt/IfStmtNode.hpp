#ifndef IFSTMTNODE_HPP_
#define IFSTMTNODE_HPP_

#include "BlockNode.hpp"

class ElseStmtNode;
class IfStmtNode: public BlockNode{
public:
    IfStmtNode(CondExprNode*, Symtable*, std::string, FunctionDeclNode*);
    virtual ~IfStmtNode();
    
    virtual std::vector<IrNode*>& translate(); 

    CondExprNode* cond;
    ElseStmtNode* elseNode;
    std::string index; // label index
};
#endif
