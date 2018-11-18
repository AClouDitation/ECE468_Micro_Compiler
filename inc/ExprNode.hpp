#ifndef EXPRNODE_HPP_
#define EXPRNODE_HPP_

#include <string>
#include <vector>
#include <stack>
#include <iostream>

class FunctionDeclNode;

class ExprNode{
protected:
    FunctionDeclNode* farther;
public:
    ExprNode(FunctionDeclNode* farther):
        farther(farther),lnode(NULL),rnode(NULL),
        type("INT"),is_var(false){};
    virtual ~ExprNode(){};
    virtual std::string translate(std::vector<std::string>&)=0;
    ExprNode* lnode;
    ExprNode* rnode;
    std::string type;
    bool is_var;    // walkaround to move <memref> <memref>
};

class AddExprNode: public ExprNode{
    char sign;
public:
    AddExprNode(FunctionDeclNode*, char);
    virtual ~AddExprNode();
    virtual std::string translate(std::vector<std::string>&);
};

class MulExprNode: public ExprNode{
    char sign;
public:
    MulExprNode(FunctionDeclNode*, char);
    virtual ~MulExprNode();
    virtual std::string translate(std::vector<std::string>&);
};

class CallExprNode: public ExprNode{
    std::string name;
public:
    CallExprNode(FunctionDeclNode*, std::string);
    virtual ~CallExprNode();
    virtual std::string translate(std::vector<std::string>&);

    std::stack<ExprNode*> exprStack;
};

class CondExprNode: public ExprNode{
    std::string out_label;
public:
    CondExprNode(FunctionDeclNode*, std::string);
    virtual ~CondExprNode();    
    void setOutLabel(std::string);
    virtual std::string translate(std::vector<std::string>&);

    std::string cmp;
};

// variable references
class VarRef: public ExprNode{
public:
    VarRef(FunctionDeclNode*, std::string, std::string);
    virtual ~VarRef();
    virtual std::string translate(std::vector<std::string>&);
    
    std::string name;
};

class LitRef: public ExprNode{
public:
    LitRef(FunctionDeclNode*, std::string, std::string);
    virtual ~LitRef();
    virtual std::string translate(std::vector<std::string>&);

    std::string value;
};

#endif