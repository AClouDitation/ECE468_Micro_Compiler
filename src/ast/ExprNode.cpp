#include "ExprNode.hpp"

int temp_reg_index;
AddExprNode::AddExprNode(char sign){
    this->sign = sign;
    this->lnode = NULL;
    this->rnode = NULL;
}

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
    string res = "$T"+to_string(temp_reg_index);
    new_ir += " " + res;

    code_block.push_back(new_ir); // only integers for now, will modify later
    temp_reg_index++;

    return res;
}

MulExprNode::MulExprNode(char sign){
    this->sign = sign;
    this->lnode = NULL;
    this->rnode = NULL;
}

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
    string res = "$T"+to_string(temp_reg_index);
    new_ir += " " + res;

    code_block.push_back(new_ir); // only integers for now, will modify later
    temp_reg_index++;

    return res;
}

// for now
CallExprNode::CallExprNode(string fname){
    this->name = fname;
    // this->arg_list = args;
}

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

    // push registers
    code_block.push_back("PUSH");
    // push a empty space to store return value of function
    code_block.push_back("PUSHREGS");
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
    // pop registers
    code_block.push_back("POPREGS");
    // pop return value
    string res = "$T"+to_string(temp_reg_index);
    code_block.push_back("POP " + res);
    return res;
}

CondExprNode::CondExprNode(string cmp):
    cmp(cmp)
{}

CondExprNode::~CondExprNode(){}

string CondExprNode::translate(vector<string>& code_block){

    string op1 = lnode->translate(code_block);
    string op2 = rnode->translate(code_block);

    //cmp op1 op2 label
    if(op2[0] != '$'){ // op2 is a regeister
        // Move it to one
        string res = "$T"+to_string(temp_reg_index);
        string new_ir = "STORE";
        if(rnode->type=="INT") new_ir+="I";
        else new_ir+="F";
        new_ir += " " + op2 + " " + res;
        code_block.push_back(new_ir);
        op2 = res;
        temp_reg_index++;
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

    // this is a design flaw
    // VarRef should not have lnode/rnode
    // however for the sake of polymorphism...
    this->lnode = NULL;
    this->rnode = NULL;
}

VarRef::~VarRef(){}
string VarRef::translate(vector<string>& code_block){return name;}  

LitRef::LitRef(string type, string val){

    this->type = type;
    this->value = val;

    // this is a design flaw
    // VarRef should not have lnode/rnode
    // however for the sake of polymorphism...
    this->lnode = NULL;
    this->rnode = NULL;
}

LitRef::~LitRef(){}
string LitRef::translate(vector<string>& code_block){return value;}
