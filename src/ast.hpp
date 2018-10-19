#ifndef AST_HPP
#define AST_HPP
#include <string>
#include <vector>
#include "../src/symtable.hpp"

using namespace std;
// expressions

class ExprNode{
public:
    ExprNode():type("INT"){};
    virtual ~ExprNode(){};
    virtual string translate(vector<string>&)=0;
    ExprNode* lnode;
    ExprNode* rnode;
    string type;
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
    string cmp;
public:
    CondExprNode(string);
    virtual ~CondExprNode();    
    virtual string translate(vector<string>&);
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

class StmtNode{
public:
    StmtNode(){};
    virtual ~StmtNode(){};
    virtual vector<string>& translate()=0;
};

class AssignStmtNode: public StmtNode{
public:
    AssignStmtNode();
    virtual ~AssignStmtNode();

    void update_AST_type(ExprNode*);
    vector<string>& translate();
    
    VarRef* to;
    ExprNode* from;
};

class WriteStmtNode: public StmtNode{
public:
    WriteStmtNode();
    virtual ~WriteStmtNode();
    vector<string>& translate();
    vector<VarRef*> id_list;
};

class ReadStmtNode: public StmtNode{
public:
    ReadStmtNode();
    virtual ~ReadStmtNode();
    vector<string>& translate();
    vector<VarRef*> id_list;
};

class FunctionDeclNode{
public:
    FunctionDeclNode(string, string, Symtable* symtable);
    virtual ~FunctionDeclNode();

    vector<string>& translate();
    vector<StmtNode*> stmt_list;    
    string name;
    string type;
    Symtable* symtable; 
};

class IfStmtNode: public StmtNode{
public:
    IfStmtNode(CondExprNode*);
    virtual ~IfStmtNode();
    string LabelName;
    string invCmp;

    CondExprNode* cond;
    vector<string>& translate(); 
};
#endif
