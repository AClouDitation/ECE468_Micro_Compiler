#include <assert.h>
#include "../../inc/ExprNode.hpp"
#include "../../inc/StmtNode.hpp"
#include "../../inc/irNode.hpp"
#include "../../inc/utility.hpp"

using namespace std;

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
