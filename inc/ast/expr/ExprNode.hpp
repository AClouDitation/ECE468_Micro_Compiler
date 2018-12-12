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
    static void updateExpr(ExprNode*);
};

void ExprNode::updateExpr(ExprNode* root) {
    if(root->lnode) updateExpr(root->lnode);
    if(root->rnode) updateExpr(root->rnode);
    
    if(!root->lnode && !root->rnode) return;

    if(root->lnode->type == "R" || root->rnode->type == "R") {
        root->type = "R";
    }
}

#endif
