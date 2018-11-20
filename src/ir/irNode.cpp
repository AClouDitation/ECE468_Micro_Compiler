#include "../../inc/irNode.hpp"
#include <iostream>

using namespace std;

/* ----- generic IR nodes ----- */
IrNode::IrNode(string cmd):
    cmd(cmd), predecessor(NULL), successor(NULL){}

IrNode::~IrNode() {}

void IrNode::print() {
    cout << cmd << endl;
}

void IrNode::setPre(IrNode* predecessor) {
   this->predecessor = predecessor; 
}

void IrNode::setSuc(IrNode* successor) {
    this->successor = successor;
}

/* ----- Arithmetic IR nodes ----- */
ArithmeticIrNode::ArithmeticIrNode(string cmd, string type, string op1, 
        string op2, string res):
        IrNode(cmd), type(type), op1(op1), op2(op2), res(res)
{}

ArithmeticIrNode::~ArithmeticIrNode() {}

void ArithmeticIrNode::print() {
    cout << cmd << type << " ";
    cout << op1 << " " << op2 << " " << res << endl;
}

/* ----- Store IR nodes ----- */
StoreIrNode::StoreIrNode(string type, string op1, string res):
    IrNode("STORE"), type(type), op1(op1), res(res) {}

StoreIrNode::~StoreIrNode() {}

void StoreIrNode::print() {
    cout << cmd << type << " ";
    cout << op1 << " " << res << endl;
}

/* ----- Read IR nodes ----- */
ReadIrNode::ReadIrNode(string type, string res):
        IrNode("READ"), type(type), res(res) {}

ReadIrNode::~ReadIrNode() {}

void ReadIrNode::print() {
    cout << cmd << type << " ";
    cout << res << endl;
}

/* ----- Write IR nodes ----- */
WriteIrNode::WriteIrNode(string type, string op1):
        IrNode("WRITE"), type(type), op1(op1) {}

WriteIrNode::~WriteIrNode() {}

void WriteIrNode::print() {
    cout << cmd << type << " ";
    cout << op1 << endl;
}

/* ----- Call IR nodes ----- */
CallIrNode::CallIrNode(string name):
    IrNode("JSR"), name(name) {}

CallIrNode::~CallIrNode() {}

void CallIrNode::print() {
    cout << cmd << " " << "FUNC_" << name << endl;
}

/* ----- Push IR nodes ----- */
PushIrNode::PushIrNode():
    IrNode("PUSH"), op1("") {}

PushIrNode::PushIrNode(string op1):
    IrNode("PUSH"), op1(op1) {}

PushIrNode::~PushIrNode() {}

void PushIrNode::print() {
    if(op1 == "!REGS") cout << cmd << "REGS" << endl;
    else cout << cmd << " " << op1 << endl;
}

/* ----- Pop IR nodes ----- */
PopIrNode::PopIrNode():
    IrNode("POP"), op1("") {}

PopIrNode::PopIrNode(string op1):
    IrNode("POP"), op1(op1) {}

PopIrNode::~PopIrNode() {}

void PopIrNode::print() {
    if(op1 == "!REGS") cout << cmd << "REGS" << endl;
    else cout << cmd << " " << op1 << endl;
}

/* ----- Jump IR nodes ----- */
JumpIrNode::JumpIrNode(string label):
    IrNode("JUMP"), label(label) {}

JumpIrNode::~JumpIrNode() {}

void JumpIrNode::print() {
    cout << cmd << " " << label << endl;
}

/* ----- Link IR nodes ----- */
LinkIrNode::LinkIrNode(int size):
    IrNode("LINK"), size(size) {}

LinkIrNode::~LinkIrNode() {}

void LinkIrNode::print() {
    cout << cmd << " " << size << endl;
}
