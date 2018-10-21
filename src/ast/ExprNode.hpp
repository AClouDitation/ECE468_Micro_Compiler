#ifndef EXPRNODE_HPP
#define EXPRNODE_HPP

#include <string>
#include <vector>

using namespace std;

class ExprNode{
public:
    ExprNode():type("INT"),is_var(false){};
    virtual ~ExprNode(){};
    virtual string translate(vector<string>&)=0;
    ExprNode* lnode;
    ExprNode* rnode;
    string type;
    bool is_var;    // walkaround to move <memref> <memref>
};

class AddExprNode: public ExprNode{
    char sign;
public:
    AddExprNode(char);
    virtual ~AddExprNode();
    virtual string translate(vector<string>&);
};

class MulExprNode: public ExprNode{
    char sign;
public:
    MulExprNode(char);
    virtual ~MulExprNode();
    virtual string translate(vector<string>&);
};

class CallExprNode: public ExprNode{
public:
    CallExprNode(string);
    virtual ~CallExprNode();
    virtual string translate(vector<string>&);

    string name;
    vector<string> arg_list;
};

class CondExprNode: public ExprNode{
public:
    CondExprNode(string);
    virtual ~CondExprNode();    
    virtual string translate(vector<string>&);

    string cmp;
};

// variable references
class VarRef: public ExprNode{
public:
    VarRef(string, string);
    virtual ~VarRef();
    virtual string translate(vector<string>&);
    
    string name;
};

class LitRef: public ExprNode{
public:
    LitRef(string, string);
    virtual ~LitRef();
    virtual string translate(vector<string>&);

    string value;
};

#endif
