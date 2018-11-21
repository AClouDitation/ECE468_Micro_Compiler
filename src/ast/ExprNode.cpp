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
    string type = this->type == "FLOAT"? "F":"I";
    string res = farther->getNextAvaTemp();

    IrNode* newIr = new ArithmeticIrNode(cmd, type, op1, op2, res);
    irBlockInsert(code_block, newIr);

    return res;
}

MulExprNode::MulExprNode(FunctionDeclNode* farther, char sign):
    ExprNode(farther), sign(sign){}

MulExprNode::~MulExprNode(){}
string MulExprNode::translate(vector<IrNode*>& code_block){
    string op1 = lnode->translate(code_block);
    string op2 = rnode->translate(code_block);

    string cmd = sign == '*'?"MUL":"DIV";
    string type = this->type == "FLOAT"? "F":"I";
    string res = farther->getNextAvaTemp();

    IrNode* newIr = new ArithmeticIrNode(cmd, type, op1, op2, res);
    irBlockInsert(code_block, newIr);

    return res;
}

// for now
CallExprNode::CallExprNode(FunctionDeclNode* farther, string fname):
    ExprNode(farther),name(fname){}

CallExprNode::~CallExprNode(){}

string CallExprNode::translate(vector<IrNode*>& code_block){

    vector<string> args;
    int argc = 0;
    // prepare arguments
    while(!exprStack.empty()){
        argc++;
        string ret = exprStack.top()->translate(code_block);
        args.push_back(ret);
        exprStack.pop();
    }

    irBlockInsert(code_block, new IrNode("PUSHREGS")); // push all register inuse
    irBlockInsert(code_block, new PushIrNode());        // push a empty space to store return value of function
    for(auto argToPush: args){                          // push arguments on stack
        irBlockInsert(code_block, new PushIrNode(argToPush));
    }
        
    irBlockInsert(code_block, new CallIrNode(name));    // call function

    for(int i = 0;i < argc;i++){                        // pop arguments
        irBlockInsert(code_block, new PopIrNode());
    }

    string res = farther->getNextAvaTemp();
    irBlockInsert(code_block, new PopIrNode(res));      // pop return value
    irBlockInsert(code_block, new IrNode("POPREGS"));  // pop registers

    return res;
}

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

    assert(out_label != "NULL");
    string op1 = lnode->translate(code_block);
    string op2 = rnode->translate(code_block);

    //cmp op1 op2 label
    if(!(op2[0] == '!' && op2[1] == 'T')){ // op2 is not a temporary
        // Move it to one
        string res = farther->getNextAvaTemp();
        string type = rnode->type == "INT"? "I":"F";                    //TODO: standardlize all type to 1 letter string
        irBlockInsert(code_block, new StoreIrNode(type, op2, res));
        op2 = res;
    }

    string type = "I";
    if(lnode->type == "FLOAT" || rnode->type == "FLOAT") type = "F";    // promote type to float if applicable    

    // out_label should be set by the caller this function
    irBlockInsert(code_block, new CondIrNode(negateCond(cmp), type, op1, op2, out_label));
    return out_label;   // this will not actually be used, just for polymorphism
}


VarRef::VarRef(FunctionDeclNode* farther, string name, string type):
    ExprNode(farther),name(name){
    this->type = type;
    this->is_var = true;
}

VarRef::~VarRef(){}

string VarRef::translate(vector<IrNode*>& code_block) {return name;}  


LitRef::LitRef(FunctionDeclNode* farther, string type, string val):
    ExprNode(farther),value(val){this->type = type;}

LitRef::~LitRef(){}

string LitRef::translate(vector<IrNode*>& code_block){return value;}
