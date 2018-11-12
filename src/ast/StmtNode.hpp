#ifndef STMTNODE_HPP
#define STMTNODE_HPP
#include <vector>
#include <string>
#include "../symtable/symtable.hpp"
#include "../ast/ExprNode.hpp"
#include "../regman/regman.hpp"

using namespace std;

class StmtNode{
public:
    StmtNode(){};
    virtual ~StmtNode(){};
    virtual vector<string>& translate(regManager&)=0;
};

class AssignStmtNode: public StmtNode{
public:
    AssignStmtNode();
    virtual ~AssignStmtNode();

    void update_AST_type(ExprNode*);
    vector<string>& translate(regManager&);
    
    VarRef* to;
    ExprNode* from;
};

class WriteStmtNode: public StmtNode{
public:
    WriteStmtNode();
    virtual ~WriteStmtNode();
    vector<string>& translate(regManager&);
    vector<VarRef*> id_list;
};

class ReadStmtNode: public StmtNode{
public:
    ReadStmtNode();
    virtual ~ReadStmtNode();
    vector<string>& translate(regManager&);
    vector<VarRef*> id_list;
};

class BlockNode: public StmtNode{
public:
    BlockNode(Symtable*);
    virtual ~BlockNode();
    virtual vector<string>& translate(regManager&)=0;

    vector<StmtNode*> stmt_list;
    Symtable* symtable;
};

class FunctionDeclNode: public BlockNode{
public:
    FunctionDeclNode(string, string, int, Symtable* symtable);
    virtual ~FunctionDeclNode();

    virtual vector<string>& translate(regManager&);
    virtual vector<string>& translate();
    string name;
    string type;
    int argc;
    regManager regMan;
};

class ElseStmtNode: public BlockNode{
public:
    ElseStmtNode(Symtable*);
    virtual ~ElseStmtNode();
    virtual vector<string>& translate(regManager&);

};

class IfStmtNode: public BlockNode{
public:
    IfStmtNode(CondExprNode*, Symtable*, string);
    virtual ~IfStmtNode();
    
    virtual vector<string>& translate(regManager&); 

    CondExprNode* cond;
    ElseStmtNode* elseNode;
    string index;
};

class WhileStmtNode: public BlockNode{
public:
    WhileStmtNode(CondExprNode*, Symtable*, string);
    virtual ~WhileStmtNode();
    vector<string>& translate(regManager&);

    CondExprNode* cond;
    string index;
};

class ReturnStmtNode: public StmtNode{
    ExprNode* expr;
    int retLoc;
public:
    ReturnStmtNode(ExprNode*, int);
    virtual ~ReturnStmtNode();
    virtual vector<string>& translate(regManager&);
};

#endif
