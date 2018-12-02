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

#endif
