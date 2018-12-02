#include <assert.h>
#include "../../inc/ExprNode.hpp"
#include "../../inc/StmtNode.hpp"
#include "../../inc/irNode.hpp"
#include "../../inc/utility.hpp"

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