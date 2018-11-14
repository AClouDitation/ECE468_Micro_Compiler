#include "ExprNode.hpp"
#include "StmtNode.hpp"

using namespace std;

AddExprNode::AddExprNode(FunctionDeclNode* farther, char sign):
    ExprNode(farther), sign(sign){}

AddExprNode::~AddExprNode(){}
string AddExprNode::translate(vector<string>& code_block){
    string op1 = lnode->translate(code_block);
    string op2 = rnode->translate(code_block);

    string new_ir = "";
    if(sign == '+') new_ir += "ADD";
    else new_ir += "SUB";

    if(type == "FLOAT") new_ir += "F ";
    else new_ir += "I ";

    new_ir += op1+" "+op2;
    string newTemp = farther->getNextAvaTemp();
    new_ir += " " + newTemp;

    code_block.push_back(new_ir); // only integers for now, will modify later

    return newTemp;
}

MulExprNode::MulExprNode(FunctionDeclNode* farther, char sign):
    ExprNode(farther), sign(sign){}

MulExprNode::~MulExprNode(){}
string MulExprNode::translate(vector<string>& code_block){
    string op1 = lnode->translate(code_block);
    string op2 = rnode->translate(code_block);

    string new_ir = "";
    if(sign == '*') new_ir += "MUL";
    else new_ir += "DIV";

    if(type == "FLOAT") new_ir += "F ";
    else new_ir += "I ";
    
    new_ir += op1+" "+op2;

    string newTemp = farther->getNextAvaTemp();
    new_ir += " " + newTemp;

    code_block.push_back(new_ir); // only integers for now, will modify later

    return newTemp;
}

// for now
CallExprNode::CallExprNode(FunctionDeclNode* farther, string fname):
    ExprNode(farther),name(fname){}

CallExprNode::~CallExprNode(){}

string CallExprNode::translate(vector<string>& code_block){

    vector<string> args;
    int argc = 0;
    // prepare arguments
    while(!exprStack.empty()){
        argc++;
        string ret = exprStack.top()->translate(code_block);
        args.push_back(ret);
        exprStack.pop();
    }

    code_block.push_back("PUSHREGS");

    // push a empty space to store return value of function
    code_block.push_back("PUSH");
    // push arguments on stack
    for(auto argToPush: args){
        code_block.push_back("PUSH " + argToPush);
    }
        
    // call function
    code_block.push_back("JSR FUNC_" + name);

    // pop arguments
    for(int i = 0;i < argc;i++){
        code_block.push_back("POP");
    }
    // pop return value
    string newTemp = farther->getNextAvaTemp();
    code_block.push_back("POP " + newTemp);
    // pop registers
    code_block.push_back("POPREGS");

    return newTemp;
}

CondExprNode::CondExprNode(FunctionDeclNode* farther, string cmp):
    ExprNode(farther),cmp(cmp)
{}

CondExprNode::~CondExprNode(){}

string CondExprNode::translate(vector<string>& code_block){

    string op1 = lnode->translate(code_block);
    string op2 = rnode->translate(code_block);

    //cmp op1 op2 label
    if(!(op2[0] == '$' && op2[1] == 'T')){ // op2 is not a regeister
        // Move it to one
        string newTemp = farther->getNextAvaTemp();
        string new_ir = "STORE";
        if(rnode->type=="INT") new_ir+="I";
        else new_ir+="F";
        new_ir += " " + op2 + " " + newTemp;
        code_block.push_back(new_ir);
        op2 = newTemp;
    }

    string type = "INT";
    if(lnode->type == "FLOAT" || rnode->type == "FLOAT"){
        type = "FLOAT";        
    }

    string new_ir = cmp + " " + type + " " + op1 + " " + op2 + " SUCCESS_";
    code_block.push_back(new_ir);
    return cmp;
}


VarRef::VarRef(FunctionDeclNode* farther, string name, string type):
    ExprNode(farther),name(name){
    this->type = type;
    this->is_var = true;
}

VarRef::~VarRef(){}

string VarRef::translate(vector<string>& code_block) {return name;}  


LitRef::LitRef(FunctionDeclNode* farther, string type, string val):
    ExprNode(farther),value(val){this->type = type;}

LitRef::~LitRef(){}

string LitRef::translate(vector<string>& code_block){return value;}
