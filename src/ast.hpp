#ifndef AST_HPP
#define AST_HPP
#include <string>
#include <vector>
#include "../src/symtable.hpp"

// expressions

static int temp_reg_index = 0;
class ExprNode{
public:
    ExprNode():type("INT"){};
    virtual ~ExprNode(){};
    virtual std::string translate(std::vector<std::string>&)=0;
    ExprNode* lnode;
    ExprNode* rnode;
    std::string type;
};

class AddExprNode: public ExprNode{
    char sign;
public:
    AddExprNode(char);
    virtual ~AddExprNode();
    virtual std::string translate(std::vector<std::string>&);
};

class MulExprNode: public ExprNode{
    char sign;
public:
    MulExprNode(char);
    virtual ~MulExprNode();
    virtual std::string translate(std::vector<std::string>&);
};

class CallExprNode: public ExprNode{
public:
    CallExprNode(std::string);
    virtual ~CallExprNode();
    virtual std::string translate(std::vector<std::string>&);

    std::string name;
    std::vector<std::string> arg_list;
};

// variable references
class VarRef: public ExprNode{
public:
    VarRef(std::string, std::string);
    virtual ~VarRef();
    virtual std::string translate(std::vector<std::string>&);
    
    std::string name;
};

class LitRef: public ExprNode{
public:
    LitRef(std::string, std::string);
    virtual ~LitRef();
    virtual std::string translate(std::vector<std::string>&);

    std::string value;
};

class StmtNode{
public:
    StmtNode(){};
    virtual ~StmtNode(){};
    virtual std::vector<std::string>& translate()=0;
};

class AssignStmtNode: public StmtNode{
public:
    AssignStmtNode();
    virtual ~AssignStmtNode();

    void update_AST_type(ExprNode*);
    std::vector<std::string>& translate();
    
    VarRef* to;
    ExprNode* from;
};

class WriteStmtNode: public StmtNode{
public:
    WriteStmtNode();
    virtual ~WriteStmtNode();
    std::vector<std::string>& translate();
    std::vector<VarRef*> id_list;
};

class ReadStmtNode: public StmtNode{
public:
    ReadStmtNode();
    virtual ~ReadStmtNode();
    std::vector<std::string>& translate();
    std::vector<VarRef*> id_list;
};

class FunctionDeclNode{
public:
    FunctionDeclNode(std::string, std::string, Symtable* symtable);
    virtual ~FunctionDeclNode();

    std::vector<std::string>& translate();
    std::vector<StmtNode*> stmt_list;    
    std::string name;
    std::string type;
    Symtable* symtable; 
};
#endif
