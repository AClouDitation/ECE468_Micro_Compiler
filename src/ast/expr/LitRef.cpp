#include "../../../inc/ast/expr/LitRef.hpp"
#include "../../../inc/ir/irNode.hpp"
#include <assert.h>

using namespace std;

LitRef::LitRef(FunctionDeclNode* farther, string type, string val):
    ExprNode(farther),value(val){this->type = type;}

LitRef::~LitRef(){}

string LitRef::translate(vector<IrNode*>& code_block){return value;}

