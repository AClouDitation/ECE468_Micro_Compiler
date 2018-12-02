#include "../../../inc/ast/expr/AddExprNode.hpp"
#include "../../../inc/ast/stmt/blk_stmt/FunctionDeclNode.hpp"
#include "../../../inc/ir/ArithmeticIrNode.hpp"
#include "../../../inc/utility.hpp"
#include <assert.h>

using namespace std;

AddExprNode::AddExprNode(FunctionDeclNode* farther, char sign):
    ExprNode(farther), sign(sign){}

AddExprNode::~AddExprNode(){}
string AddExprNode::translate(vector<IrNode*>& code_block){
    string op1 = lnode->translate(code_block);
    string op2 = rnode->translate(code_block);

    string cmd = sign == '+'?"ADD":"SUB";
    string type = this->type;
    string res = farther->getNextAvaTemp();

    IrNode* newIr = new ArithmeticIrNode(cmd, type, op1, op2, res, *(farther->regMan));
    irBlockInsert(code_block, newIr);

    return res;
}
