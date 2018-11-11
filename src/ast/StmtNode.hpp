#ifndef STMTNODE_HPP
#define STMTNODE_HPP
#include <vector>
#include <string>
#include "../symtable/symtable.hpp"
#include "../ast/ExprNode.hpp"

using namespace std;

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

class BlockNode: public StmtNode{
public:
    BlockNode(Symtable*);
    virtual ~BlockNode();
    virtual vector<string>& translate()=0;

    vector<StmtNode*> stmt_list;
    Symtable* symtable;
};

class FunctionDeclNode: public BlockNode{
public:
    FunctionDeclNode(string, string, Symtable* symtable);
    virtual ~FunctionDeclNode();

    virtual vector<string>& translate();
    string name;
    string type;
};

class ElseStmtNode: public BlockNode{
public:
    ElseStmtNode(Symtable*);
    virtual ~ElseStmtNode();
    virtual vector<string>& translate();

};

class IfStmtNode: public BlockNode{
public:
    IfStmtNode(CondExprNode*, Symtable*, string);
    virtual ~IfStmtNode();
    
    virtual vector<string>& translate(); 

    CondExprNode* cond;
    ElseStmtNode* elseNode;
    string index;
};

class WhileStmtNode: public BlockNode{
public:
    WhileStmtNode(CondExprNode*, Symtable*, string);
    virtual ~WhileStmtNode();
    vector<string>& translate();

    CondExprNode* cond;
    string index;
};

class ReturnStmtNode: public StmtNode{
    ExprNode* expr;
public:
    ReturnStmtNode(ExprNode*);
    virtual ~ReturnStmtNode();
    virtual vector<string>& translate();
};

#endif

