#include "../../inc/ir/JumpIrNode.hpp"
#include "../../inc/utility.hpp"
#include <assert.h>

using namespace std;

/* ----- Jump IR nodes ----- */
JumpIrNode::JumpIrNode(string label, regManager& regMan):
    IrNode("JUMP", regMan), label(label) {}

JumpIrNode::~JumpIrNode() {}

stringstream JumpIrNode::print() {
    stringstream ss = IrNode::print();
    ss << " " << label;
    return ss;
}

vector<string> JumpIrNode::translate() {
    vector<string> opCodeBlock;
    // spill everything before jump
    regMan.regFree(0, opCodeBlock, outSet);
    regMan.regFree(1, opCodeBlock, outSet);
    regMan.regFree(2, opCodeBlock, outSet);
    regMan.regFree(3, opCodeBlock, outSet);
    opCodeBlock.push_back("jmp " + label);
    return opCodeBlock;
}

