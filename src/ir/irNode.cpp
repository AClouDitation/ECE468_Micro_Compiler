#include "../../inc/irNode.hpp"
#include "../../inc/utility.hpp"
#include <iostream>
#include <queue>

using namespace std;

queue<IrNode*> IrNode::worklist;

/* ----- generic IR nodes ----- */
IrNode::IrNode(string cmd):
    cmd(cmd), predecessor(NULL), successor(NULL){
        IrNode::worklist.push(this); 
}

IrNode::~IrNode() {}

void IrNode::print() {
    cout << cmd;
}

void IrNode::setPre(IrNode* predecessor) {
    this->predecessor = predecessor; 
}

void IrNode::setSuc(IrNode* successor) {
    this->successor = successor;
}

void IrNode::insertOutSet(string id) {
    if(!isLiteral(id)) outSet.insert(id);
}

void IrNode::insertInSet(string id) {
    if(!isLiteral(id)) inSet.insert(id);
}

void IrNode::printGen() {
    cout << "GEN SET: ";
    for(auto id: genSet){
        cout << id << " ";
    }
}

void IrNode::printKill() {
    cout << "KILL SET: ";
    for(auto id: killSet){
        cout << id << " ";
    }
}

void IrNode::livenessCalc() {
    if(successor) outSet = successor->inSet;
    inSet = outSet;
    for(auto id: killSet) inSet.erase(id);
    for(auto id: genSet)  inSet.insert(id);
}

/* ----- Arithmetic IR nodes ----- */
ArithmeticIrNode::ArithmeticIrNode(string cmd, string type, string op1, 
    string op2, string res):
    IrNode(cmd), type(type), op1(op1), op2(op2), res(res) {
    if(!isLiteral(op1)) genSet.insert(op1);
    if(!isLiteral(op2)) genSet.insert(op2);
    if(!isLiteral(res)) killSet.insert(res);
}

ArithmeticIrNode::~ArithmeticIrNode() {}

void ArithmeticIrNode::print() {
    cout << cmd << type << " ";
    cout << op1 << " " << op2 << " " << res;
}

/* ----- Store IR nodes ----- */
StoreIrNode::StoreIrNode(string type, string op1, string res):
    IrNode("STORE"), type(type), op1(op1), res(res) {
    if(!isLiteral(op1)) genSet.insert(op1);
    if(!isLiteral(res)) killSet.insert(res);
}

StoreIrNode::~StoreIrNode() {}

void StoreIrNode::print() {
    cout << cmd << type << " ";
    cout << op1 << " " << res;
}

/* ----- Read IR nodes ----- */
ReadIrNode::ReadIrNode(string type, string res):
    IrNode("READ"), type(type), res(res) {
    if(!isLiteral(res)) killSet.insert(res);
}

ReadIrNode::~ReadIrNode() {}

void ReadIrNode::print() {
    cout << cmd << type << " ";
    cout << res;
}

/* ----- Write IR nodes ----- */
WriteIrNode::WriteIrNode(string type, string op1):
    IrNode("WRITE"), type(type), op1(op1) {
    if(!isLiteral(op1)) genSet.insert(op1);
}

WriteIrNode::~WriteIrNode() {}

void WriteIrNode::print() {
    cout << cmd << type << " ";
    cout << op1;
}

/* ----- Call IR nodes ----- */
CallIrNode::CallIrNode(string name):
    IrNode("JSR"), name(name) {}

CallIrNode::~CallIrNode() {}

void CallIrNode::print() {
    cout << cmd << " " << "FUNC_" << name;
}

/* ----- Push IR nodes ----- */
PushIrNode::PushIrNode():
    IrNode("PUSH"), op1("") {}

PushIrNode::PushIrNode(string op1):
    IrNode("PUSH"), op1(op1) {
    if(!isLiteral(op1)) genSet.insert(op1);
}

PushIrNode::~PushIrNode() {}

void PushIrNode::print() {
    if(op1 == "!REGS") cout << cmd << "REGS";
    else cout << cmd << " " << op1;
}

/* ----- Pop IR nodes ----- */
PopIrNode::PopIrNode():
    IrNode("POP"), op1("") {}

PopIrNode::PopIrNode(string op1):
    IrNode("POP"), op1(op1) {
    if(!isLiteral(op1)) killSet.insert(op1);
}

PopIrNode::~PopIrNode() {}

void PopIrNode::print() {
    if(op1 == "!REGS") cout << cmd << "REGS";
    else cout << cmd << " " << op1;
}

/* ----- Jump IR nodes ----- */
JumpIrNode::JumpIrNode(string label):
    IrNode("JUMP"), label(label) {}

JumpIrNode::~JumpIrNode() {}

void JumpIrNode::print() {
    cout << cmd << " " << label;
}

/* ----- Link IR nodes ----- */
LinkIrNode::LinkIrNode(int size):
    IrNode("LINK"), size(size) {}

LinkIrNode::~LinkIrNode() {}

void LinkIrNode::print() {
    cout << cmd << " " << size;
}

