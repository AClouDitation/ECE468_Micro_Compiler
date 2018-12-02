#ifndef WRITESTMTNODE_HPP_
#define WRITESTMTNODE_HPP_

#include <string>
#include <vector>
#include "BaseStmtNode.hpp"

class ExprNode;
class VarRef;


class WriteStmtNode: public BaseStmtNode{
public:
    WriteStmtNode(FunctionDeclNode*);
    virtual ~WriteStmtNode();
    std::vector<IrNode*>& translate();
    std::vector<VarRef*> id_list;
};

#endif