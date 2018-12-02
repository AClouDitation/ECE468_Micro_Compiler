#include "../../../inc/ast/expr/VarRef.hpp"
#include "../../../inc/ir/irNode.hpp"
#include <assert.h>

using namespace std;

VarRef::VarRef(FunctionDeclNode* farther, string name, string type):
    ExprNode(farther),name(name){
    this->type = type;
    this->is_var = true;
}

VarRef::~VarRef(){}

string VarRef::translate(vector<IrNode*>& code_block) {return name;}  

