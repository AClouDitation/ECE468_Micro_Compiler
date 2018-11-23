#ifndef STMTNODE_HPP_
#define STMTNODE_HPP_
#include <vector>
#include <string>
#include <set>

class Symtable;
class CondExprNode;
class IrNode;
class regManager;
class FunctionDeclNode;

class StmtNode{
protected:
    FunctionDeclNode* farther;
public:
    std::set<std::string> symGen;
    std::set<std::string> symKill;
    StmtNode(FunctionDeclNode*);
    virtual ~StmtNode();
    virtual std::vector<IrNode*>& translate()=0;
};

class BlockNode: public StmtNode{
public:
    BlockNode(Symtable*, FunctionDeclNode*);
    virtual ~BlockNode();
    virtual std::vector<IrNode*>& translate()=0;

    std::vector<StmtNode*> stmt_list;
    Symtable* symtable;
};

class FunctionDeclNode: public BlockNode{
    int nextAvaTemp;
public:
    std::string name;
    std::string type;
    
    int argc;
    int stackSize;

    regManager* regMan;
    FunctionDeclNode(std::string, std::string, int, Symtable* symtable);
    virtual ~FunctionDeclNode();

    virtual std::vector<IrNode*>& translate();

    std::string getNextAvaTemp();
    int getStackSize();
};


class ElseStmtNode: public BlockNode{
public:
    ElseStmtNode(Symtable*, FunctionDeclNode*);
    virtual ~ElseStmtNode();
    virtual std::vector<IrNode*>& translate();

};

class IfStmtNode: public BlockNode{
public:
    IfStmtNode(CondExprNode*, Symtable*, std::string, FunctionDeclNode*);
    virtual ~IfStmtNode();
    
    virtual std::vector<IrNode*>& translate(); 

    CondExprNode* cond;
    ElseStmtNode* elseNode;
    std::string index; // label index
};

class WhileStmtNode: public BlockNode{
public:
    WhileStmtNode(CondExprNode*, Symtable*, std::string, FunctionDeclNode*);
    virtual ~WhileStmtNode();
    std::vector<IrNode*>& translate();

    CondExprNode* cond;
    std::string index;
};

#endif
