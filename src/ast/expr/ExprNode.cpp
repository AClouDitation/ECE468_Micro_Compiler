#include "../../../inc/ast/expr/ExprNode.hpp"
void ExprNode::updateExpr(ExprNode* root) {
    if(root->lnode) updateExpr(root->lnode);
    if(root->rnode) updateExpr(root->rnode);
    
    if(!root->lnode && !root->rnode) return;

    if(root->lnode->type == "R" || root->rnode->type == "R") {
        root->type = "R";
    }
}
