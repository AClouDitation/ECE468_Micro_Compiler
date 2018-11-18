#ifndef STMTNODE_HPP_
#define STMTNODE_HPP_
#include <vector>
#include <string>

class Symtable;
class CondExprNode;

class StmtNode{
public:
    StmtNode(){};
    virtual ~StmtNode(){};
    virtual std::vector<std::string>& translate()=0;
};

class BlockNode: public StmtNode{
public:
    BlockNode(Symtable*);
    virtual ~BlockNode();
    virtual std::vector<std::string>& translate()=0;

    std::vector<StmtNode*> stmt_list;
    Symtable* symtable;
};

class FunctionDeclNode: public BlockNode{
    int nextAvaTemp;
public:
    FunctionDeclNode(std::string, std::string, int, Symtable* symtable);
    virtual ~FunctionDeclNode();

    virtual std::vector<std::string>& translate();
    std::string name;
    std::string type;
    int argc;

    std::string getNextAvaTemp();
};


class ElseStmtNode: public BlockNode{
public:
    ElseStmtNode(Symtable*);
    virtual ~ElseStmtNode();
    virtual std::vector<std::string>& translate();

};

class IfStmtNode: public BlockNode{
public:
    IfStmtNode(CondExprNode*, Symtable*, std::string);
    virtual ~IfStmtNode();
    
    virtual std::vector<std::string>& translate(); 

    CondExprNode* cond;
    ElseStmtNode* elseNode;
    std::string index; // label index
};

class WhileStmtNode: public BlockNode{
public:
    WhileStmtNode(CondExprNode*, Symtable*, std::string);
    virtual ~WhileStmtNode();
    std::vector<std::string>& translate();

    CondExprNode* cond;
    std::string index;
};

#endif
