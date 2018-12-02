#include "../../inc/ir/PushIrNode.hpp"
#include "../../inc/utility.hpp"
#include <assert.h>

using namespace std;

/* ----- Push IR nodes ----- */
PushIrNode::PushIrNode(regManager& regMan):
    IrNode("PUSH", regMan), op1("") {}

PushIrNode::PushIrNode(string op1, regManager& regMan):
    IrNode("PUSH", regMan), op1(op1) {
    if(!isLiteral(op1)) genSet.insert(op1);
}

PushIrNode::~PushIrNode() {}

stringstream PushIrNode::print() {
    stringstream ss = IrNode::print();
    ss << " " << op1;
    return ss;
}

vector<string> PushIrNode::translate() {
    vector<string> opCodeBlock;
    if(op1 == "") {
        opCodeBlock.push_back("push");
        return opCodeBlock;
    }

    int regX = regMan.regEnsure(op1, -1, opCodeBlock, outSet);
    if(outSet.find(op1) == outSet.end()) regMan.regFree(regX, opCodeBlock, outSet);
    opCodeBlock.push_back("push r" + to_string(regX));
    return opCodeBlock;
}

