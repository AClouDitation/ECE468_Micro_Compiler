#include "../../inc/ir/CallIrNode.hpp"
#include <assert.h>

using namespace std;

/* ----- Call IR nodes ----- */
CallIrNode::CallIrNode(string name, regManager& regMan):
    IrNode("JSR", regMan), name(name) {}

CallIrNode::~CallIrNode() {}

stringstream CallIrNode::print() {
    stringstream ss = IrNode::print();
    ss << " " << "FUNC_" << name;
    return ss;
}

vector<string> CallIrNode::translate() {
    vector<string> opCodeBlock;
    opCodeBlock.push_back("jsr FUNC_" + name);
    return opCodeBlock;
}

