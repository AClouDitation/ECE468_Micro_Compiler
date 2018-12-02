#include "../../inc/ir/PopIrNode.hpp"
#include "../../inc/utility.hpp"
#include <assert.h>

using namespace std;

/* ----- Pop IR nodes ----- */
PopIrNode::PopIrNode(regManager& regMan):
    IrNode("POP", regMan), op1("") {}

PopIrNode::PopIrNode(string op1, regManager& regMan):
    IrNode("POP", regMan), op1(op1) {
    if(!isLiteral(op1)) killSet.insert(op1);
}

PopIrNode::~PopIrNode() {}

stringstream PopIrNode::print() {
    stringstream ss = IrNode::print();
    ss << " " << op1;
    return ss;
}

vector<string> PopIrNode::translate() {
    vector<string> opCodeBlock;
    if(op1 == "") {
        opCodeBlock.push_back("pop");
        return opCodeBlock;
    }
    int regZ = regMan.regAllocate(op1, -1, opCodeBlock, outSet);
    regMan.markDirty(regZ);
    opCodeBlock.push_back("pop r" + to_string(regZ));
    return opCodeBlock;
}
