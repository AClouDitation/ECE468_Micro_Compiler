#ifndef BASESTMTNODE_HPP_
#define BASESTMTNODE_HPP_

#include <string>
#include <vector>
#include "StmtNode.hpp"

class ExprNode;
class VarRef;

class BaseStmtNode: public StmtNode {
protected:
    FunctionDeclNode* farther;
public:
    BaseStmtNode(FunctionDeclNode*);
    virtual ~BaseStmtNode();
    std::vector<IrNode*>& translate()=0;
};

class AssignStmtNode: public BaseStmtNode{
public:
    AssignStmtNode(FunctionDeclNode*);
    virtual ~AssignStmtNode();

    void update_AST_type(ExprNode*);
    std::vector<IrNode*>& translate();
    
    VarRef* to;
    ExprNode* from;
};

class WriteStmtNode: public BaseStmtNode{
public:
    WriteStmtNode(FunctionDeclNode*);
    virtual ~WriteStmtNode();
    std::vector<IrNode*>& translate();
    std::vector<VarRef*> id_list;
};

class ReadStmtNode: public BaseStmtNode{
public:
    ReadStmtNode(FunctionDeclNode*);
    virtual ~ReadStmtNode();
    std::vector<IrNode*>& translate();
    std::vector<VarRef*> id_list;
};

// control stmt
// TODO: separate control statment nodes
// TODO: add break/continue
class ReturnStmtNode: public BaseStmtNode{
    ExprNode* expr;
    int retLoc;
public:
    ReturnStmtNode(FunctionDeclNode*, ExprNode*, int);
    virtual ~ReturnStmtNode();
    virtual std::vector<IrNode*>& translate();
};


#endif
