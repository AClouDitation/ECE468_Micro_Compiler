#include "ExprNode.hpp"

using namespace std;

AddExprNode::AddExprNode(char sign){
    this->sign = sign;
}

AddExprNode::~AddExprNode(){}
string AddExprNode::translate(vector<string>& code_block, regManager& regMan){
    string op1 = lnode->translate(code_block, regMan);
    string op2 = rnode->translate(code_block, regMan);

    string new_ir = "";
    if(sign == '+') new_ir += "ADD";
    else new_ir += "SUB";

    if(type == "FLOAT") new_ir += "F ";
    else new_ir += "I ";

    new_ir += op1+" "+op2;
    //string res = "$T"+to_string(temp_reg_index);
    string newReg = regMan.takeReg();
    new_ir += " " + newReg;

    code_block.push_back(new_ir); // only integers for now, will modify later

    return newReg;
}

MulExprNode::MulExprNode(char sign){
    this->sign = sign;
}

MulExprNode::~MulExprNode(){}
string MulExprNode::translate(vector<string>& code_block, regManager& regMan){
    string op1 = lnode->translate(code_block, regMan);
    string op2 = rnode->translate(code_block, regMan);

    string new_ir = "";
    if(sign == '*') new_ir += "MUL";
    else new_ir += "DIV";

    if(type == "FLOAT") new_ir += "F ";
    else new_ir += "I ";
    
    new_ir += op1+" "+op2;
    //string res = "$T"+to_string(temp_reg_index);
    string newReg = regMan.takeReg();
    new_ir += " " + newReg;

    code_block.push_back(new_ir); // only integers for now, will modify later

    return newReg;
}

// for now
CallExprNode::CallExprNode(string fname):
    name(fname){}

CallExprNode::~CallExprNode(){}

string CallExprNode::translate(vector<string>& code_block, regManager& regMan){

    vector<string> args;
    int argc = 0;
    // prepare arguments
    while(!exprStack.empty()){
        argc++;
        string ret = exprStack.top()->translate(code_block, regMan);
        args.push_back(ret);
        exprStack.pop();
    }

    // push registers
    vector<string> inUseRegs = regMan.inUseList();
    for(auto reg: inUseRegs){
        code_block.push_back("PUSH " + reg);
    }

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
    string newReg = regMan.takeReg();
    code_block.push_back("POP " + newReg);
    // pop registers
    for(vector<string>::reverse_iterator rit = inUseRegs.rbegin();
            rit != inUseRegs.rend(); rit++){
        code_block.push_back("POP " + *rit);
    }
    return newReg;
}

CondExprNode::CondExprNode(string cmp):
    cmp(cmp)
{}

CondExprNode::~CondExprNode(){}

string CondExprNode::translate(vector<string>& code_block, regManager& regMan){

    string op1 = lnode->translate(code_block, regMan);
    string op2 = rnode->translate(code_block, regMan);

    //cmp op1 op2 label
    if(!(op2[0] == '$' && op2[1] == 'T')){ // op2 is not a regeister
        // Move it to one
        string newReg = regMan.takeReg();
        string new_ir = "STORE";
        if(rnode->type=="INT") new_ir+="I";
        else new_ir+="F";
        new_ir += " " + op2 + " " + newReg;
        code_block.push_back(new_ir);
        op2 = newReg;
    }

    string type = "INT";
    if(lnode->type == "FLOAT" || rnode->type == "FLOAT"){
        type = "FLOAT";        
    }

    string new_ir = cmp + " " + type + " " + op1 + " " + op2 + " SUCCESS_";
    code_block.push_back(new_ir);
    return cmp;
}


VarRef::VarRef(string name, string type){
    this->name = name;
    this->type = type;
    this->is_var = true;
}

VarRef::~VarRef(){}
string VarRef::translate(vector<string>& code_block, regManager& regMan) {
    return name;
}  

LitRef::LitRef(string type, string val){
    this->type = type;
    this->value = val;
}

LitRef::~LitRef(){}
string LitRef::translate(vector<string>& code_block, regManager& regMan){return value;}
