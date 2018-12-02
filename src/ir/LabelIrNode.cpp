#include "../../inc/ir/LabelIrNode.hpp"

using namespace std;

/* ----- label IR nodes ----- */
LabelIrNode::LabelIrNode(string label, regManager& regMan):
    IrNode("LABEL", regMan), predecessor2(NULL), label(label) {}

LabelIrNode::~LabelIrNode() {}

stringstream LabelIrNode::print() {
    stringstream ss = IrNode::print();
    ss << " " << label;
    return ss;
}

void LabelIrNode::setPre2(IrNode* predecessor2) {this->predecessor2 = predecessor2;}

void LabelIrNode::updateWorklist() {

    IrNode::updateWorklist();
    if(!predecessor2) return;
    for(auto irNode: worklist) if(irNode == predecessor2) return;

    worklist.push_back(predecessor2);
    predecessor2->updateWorklist();
}

vector<string> LabelIrNode::translate() {
    vector<string> opCodeBlock;
    // spill everything after label
    regMan.regFree(0, opCodeBlock, outSet);
    regMan.regFree(1, opCodeBlock, outSet);
    regMan.regFree(2, opCodeBlock, outSet);
    regMan.regFree(3, opCodeBlock, outSet);
    opCodeBlock.push_back("label " + label); 
    return opCodeBlock;
}
