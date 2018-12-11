#include "../../../inc/ast/expr/CondExprNode.hpp"
#include "../../../inc/ast/stmt/blk_stmt/FunctionDeclNode.hpp"
#include "../../../inc/ir/StoreIrNode.hpp"
#include "../../../inc/ir/CondIrNode.hpp"
#include "../../../inc/ir/JumpIrNode.hpp"
#include "../../../inc/utility.hpp"
#include <iostream>
#include <assert.h>

using namespace std;


CondExprNode::CondExprNode(FunctionDeclNode* farther, string cmp):
    ExprNode(farther),out_label("NULL"),cmp(cmp)
{}

CondExprNode::~CondExprNode(){}

string negateCond(string cond) {
    if(cond == "EQ") return "NE";
    if(cond == "NE") return "EQ";
    if(cond == "LT") return "GE";
    if(cond == "LE") return "GT";
    if(cond == "GT") return "LE";
    if(cond == "GE") return "LT";
    cerr << "ERR: unknown condition " << cond << endl;
    exit(1);
    return "ERR!!!"; // let g++ shut up
}

void CondExprNode::setOutLabel(string out_label) {this -> out_label = out_label;}

string CondExprNode::translate(vector<IrNode*>& code_block) {

    if(cmp == "TRUE" || cmp == "FALSE") {
        // out_label should be set by the caller of this function
        irBlockInsert(code_block, new CondIrNode(cmp, type, "", "",
                    out_label, *(farther->regMan)));
        return out_label;
    }

    assert(out_label != "NULL");
    string op1 = lnode->translate(code_block);
    string op2 = rnode->translate(code_block);

    //cmp op1 op2 label
    if(!(op2[0] == '!' && op2[1] == 'T')) { // op2 is not a temporary
        // Move it to one
        string res = farther->getNextAvaTemp();
        string type = rnode->type;                    
        irBlockInsert(code_block, new StoreIrNode(type, op2, res, *(farther->regMan)));
        op2 = res;
    }

    string type = "I";
    if(lnode->type == "R" || rnode->type == "R") type = "R";    // promote type to float if applicable    

    // out_label should be set by the caller of this function
    irBlockInsert(code_block, new CondIrNode(negateCond(cmp), type, op1, op2, 
                out_label, *(farther->regMan)));
    return out_label;   // this will not actually be used, just for polymorphism
}
