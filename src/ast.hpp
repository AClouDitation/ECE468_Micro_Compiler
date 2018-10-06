#ifndef AST_HPP
#define AST_HPP
#include <string>
#include <vector>

// expressions
class ExprNode{
public:
    ExprNode(){};
    virtual ~ExprNode(){};
    ExprNode* lnode;
    ExprNode* rnode;
};

class AddExprNode: public ExprNode{
    char type;
public:
    AddExprNode(char);
    virtual ~AddExprNode();
};

class MulExprNode: public ExprNode{
    char type;
public:
    MulExprNode(char);
    virtual ~MulExprNode();
};

class CallExprNode: public ExprNode{
public:
    CallExprNode(std::string);
    virtual ~CallExprNode();

    std::string name;
    std::vector<std::string> arg_list;
};

// variable references
class VarRef: public ExprNode{
public:
    VarRef(std::string, std::string);
    virtual ~VarRef();
    
    std::string type;
    std::string name;
};

class LitRef: public ExprNode{
public:
    LitRef(std::string, std::string);
    virtual ~LitRef();

    std::string type;
    std::string value;
};

class StmtNode{
public:
    StmtNode(){};
    virtual ~StmtNode(){};
};

class AssignStmtNode: public StmtNode{
public:
    AssignStmtNode();
    virtual ~AssignStmtNode();
    
    VarRef* to;
    ExprNode* from;
};

#endif
