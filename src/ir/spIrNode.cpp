#include "../../inc/irNode.hpp"
#include "../../inc/utility.hpp"

using namespace std;

/* ----- conditional IR nodes ----- */
CondIrNode::CondIrNode(string cond, string type,
    string op1, string op2, string jumpTo):
    IrNode(cond), successor2(NULL), cond(cond), type(type), 
    op1(op1), op2(op2), jumpTo(jumpTo) {
    
    if(!isLiteral(op1)) genSet.insert(op1);
    if(!isLiteral(op2)) genSet.insert(op2);
}

CondIrNode::~CondIrNode() {}

stringstream CondIrNode::print() {
    stringstream ss = IrNode::print();
    ss << type << " " << op1 << " " << op2 << " " << jumpTo;
    return ss;
}

void CondIrNode::setSuc2(IrNode* successor2) {this->successor2 = successor2;}

bool CondIrNode::livenessCalc() {
    if(successor) outSet = successor->inSet;    // initialize outSet
    set<string> inSetbk = inSet;                // backup inSet

    inSet = outSet;                             // initialize inSet

    /* calculate new inSet */
    if(successor2) for(auto id: successor2->inSet) inSet.insert(id);
    for(auto id: killSet) inSet.erase(id);
    for(auto id: genSet)  inSet.insert(id);

    return inSet != inSetbk;
}

/* ----- label IR nodes ----- */
LabelIrNode::LabelIrNode(string label):
    IrNode("LABEL"), predecessor2(NULL), label(label) {}

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
