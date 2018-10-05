#ifndef AST_HPP
#define AST_HPP
#include <string>

// expressions
class ExprNode{
public:
    ExprNode();
    virtual ~ExprNode();
    ExprNode* lnode;
    ExprNode* rnode;
    char type;
};

class AddExprNode: public ExprNode{
public:
    AddExprNode(char);
    virtual ~AddExprNode();
};

class MulExprNode: public ExprNode{
public:
    MulExprNode(char);
    virtual ~MulExprNode();
};

// variable references
class VarRef{
public:
    VarRef(std::string, std::string);
    virtual ~VarRef();
    
    std::string type;
    std::string name;
};

class StmtNode{
public:
    StmtNode();
    virtual ~StmtNode();
};

class AssignStmtNode: public StmtNode{
public:
    AssignStmtNode();
    virtual ~AssignStmtNode();
    
    VarRef* to;
    ExprNode* from;
};
#endif
