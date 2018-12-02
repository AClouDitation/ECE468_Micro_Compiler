#ifndef EXPRNODE_HPP_
#define EXPRNODE_HPP_

#include <string>
#include <vector>
#include <set>


class FunctionDeclNode;
class IrNode;

class ExprNode{
protected:
    FunctionDeclNode* farther;
    std::set<std::string> sym;
public:
    ExprNode(FunctionDeclNode* farther):
        farther(farther),lnode(NULL),rnode(NULL),
        type("I"),is_var(false){};
    virtual ~ExprNode(){};
    virtual std::string translate(std::vector<IrNode*>&)=0;
    ExprNode* lnode;
    ExprNode* rnode;
    std::string type;
    bool is_var;    // walkaround to move <memref> <memref>
};

#endif
