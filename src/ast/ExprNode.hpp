#ifndef EXPRNODE_HPP_
#define EXPRNODE_HPP_

#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include "../regman/regman.hpp"

class ExprNode{
public:
    ExprNode():lnode(NULL),rnode(NULL),type("INT"),is_var(false){};
    virtual ~ExprNode(){};
    virtual std::string translate(std::vector<std::string>&, regManager&)=0;
    ExprNode* lnode;
    ExprNode* rnode;
    std::string type;
    bool is_var;    // walkaround to move <memref> <memref>
};

class AddExprNode: public ExprNode{
    char sign;
public:
    AddExprNode(char);
    virtual ~AddExprNode();
    virtual std::string translate(std::vector<std::string>&, regManager&);
};

class MulExprNode: public ExprNode{
    char sign;
public:
    MulExprNode(char);
    virtual ~MulExprNode();
    virtual std::string translate(std::vector<std::string>&, regManager&);
};

class CallExprNode: public ExprNode{
    std::string name;
public:
    CallExprNode(std::string);
    virtual ~CallExprNode();
    virtual std::string translate(std::vector<std::string>&, regManager&);

    std::stack<ExprNode*> exprStack;
};

class CondExprNode: public ExprNode{
public:
    CondExprNode(std::string);
    virtual ~CondExprNode();    
    virtual std::string translate(std::vector<std::string>&, regManager&);

    std::string cmp;
};

// variable references
class VarRef: public ExprNode{
public:
    VarRef(std::string, std::string);
    virtual ~VarRef();
    virtual std::string translate(std::vector<std::string>&, regManager&);
    
    std::string name;
};

class LitRef: public ExprNode{
public:
    LitRef(std::string, std::string);
    virtual ~LitRef();
    virtual std::string translate(std::vector<std::string>&, regManager&);

    std::string value;
};

#endif
