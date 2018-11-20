#include "../../inc/irNode.hpp"
#include <iostream>

using namespace std;

/* ----- split node ----- */
splitIrNode::splitIrNode(string cmd):
        IrNode(cmd), successor2(NULL) {}

splitIrNode::~splitIrNode() {}

void splitIrNode::print() {}

void splitIrNode::setSuc2(IrNode* successor2) {this->successor2 = successor2;}

/* ----- merge node ----- */
mergeIrNode::mergeIrNode(string cmd):
        IrNode(cmd), predecessor2(NULL) {}

mergeIrNode::~mergeIrNode() {}

void mergeIrNode::print() {}

void mergeIrNode::setPre2(IrNode* predecessor2) {this->predecessor2 = predecessor2;}

/* ----- conditional IR nodes ----- */
CondIrNode::CondIrNode(string cond, string type,
        string op1, string op2, string jumpTo):
    splitIrNode(cond), cond(cond), type(type), op1(op1), op2(op2), jumpTo(jumpTo) {}

CondIrNode::~CondIrNode() {}

void CondIrNode::print() {
    cout << cond << type << " ";
    cout << op1 << " " << op2 << " " << jumpTo << endl;
}

/* ----- label IR nodes ----- */
LabelIrNode::LabelIrNode(string label):
    mergeIrNode("LABEL"), label(label) {}

LabelIrNode::~LabelIrNode() {}

void LabelIrNode::print() {
    cout << cmd << " " << label << endl;
}

