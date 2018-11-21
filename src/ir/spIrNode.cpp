#include "../../inc/irNode.hpp"
#include "../../inc/utility.hpp"
#include <iostream>

using namespace std;

/* ----- split node ----- */
splitIrNode::splitIrNode(string cmd):
        IrNode(cmd), successor2(NULL) {}

splitIrNode::~splitIrNode() {}

void splitIrNode::print() {}

void splitIrNode::setSuc2(IrNode* successor2) {this->successor2 = successor2;}

void splitIrNode::livenessCalc() {
    if(successor) outSet = successor->inSet;
    inSet = outSet;

    if(successor2) for(auto id: successor2->inSet) inSet.insert(id);
    for(auto id: killSet) inSet.erase(id);
    for(auto id: genSet)  inSet.insert(id);
}

/* ----- merge node ----- */
mergeIrNode::mergeIrNode(string cmd):
        IrNode(cmd), predecessor2(NULL) {}

mergeIrNode::~mergeIrNode() {}

void mergeIrNode::print() {}

void mergeIrNode::setPre2(IrNode* predecessor2) {this->predecessor2 = predecessor2;}

/* ----- conditional IR nodes ----- */
CondIrNode::CondIrNode(string cond, string type,
    string op1, string op2, string jumpTo):
    splitIrNode(cond), cond(cond), type(type), op1(op1), op2(op2), jumpTo(jumpTo) {
    
    if(!isLiteral(op1)) genSet.insert(op1);
    if(!isLiteral(op2)) genSet.insert(op2);
}

CondIrNode::~CondIrNode() {}

void CondIrNode::print() {
    cout << cond << type << " ";
    cout << op1 << " " << op2 << " " << jumpTo;
}

/* ----- label IR nodes ----- */
LabelIrNode::LabelIrNode(string label):
    mergeIrNode("LABEL"), label(label) {}

LabelIrNode::~LabelIrNode() {}

void LabelIrNode::print() {
    cout << cmd << " " << label;
}

