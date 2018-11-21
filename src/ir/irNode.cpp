#include "../../inc/irNode.hpp"
#include "../../inc/utility.hpp"
#include <algorithm>
#include <iomanip>

using namespace std;

list<IrNode*> IrNode::worklist;

/* ----- generic IR nodes ----- */
IrNode::IrNode(string cmd):
    cmd(cmd), predecessor(NULL), successor(NULL){
        IrNode::worklist.push_back(this); 
}

IrNode::~IrNode() {}

stringstream IrNode::print() {
    stringstream ss;
    ss << cmd;
    return ss;
}

void IrNode::reformatPrint() {
    stringstream ss;
    cout << left << setfill(' ') << setw(40) << print().str();
    cout << left << setfill(' ') << setw(40) << printIn().str();
    cout << left << setfill(' ') << setw(40) << printOut().str();
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

stringstream IrNode::printIn() {
    stringstream ss;
    ss << "In SET: ";
    for(auto id: inSet){
        ss << id << " ";
    }
    return ss;
}

stringstream IrNode::printOut() {
    stringstream ss;
    ss << "Out SET: ";
    for(auto id: outSet){
        ss << id << " ";
    }
    return ss;
}

bool IrNode::livenessCalc() {
    if(successor) outSet = successor->inSet;    // initialize outSet
    set<string> inSetbk = inSet;                // backup inSet

    inSet = outSet;                             // initialize inSet                    

    /* calculate new inSet */
    for(auto id: killSet) inSet.erase(id);      
    for(auto id: genSet)  inSet.insert(id);

    return inSet != inSetbk;
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

stringstream ArithmeticIrNode::print() {
    stringstream ss = IrNode::print();
    ss << type << " " << op1 << " " << op2 << " " << res;
    return ss;
}

/* ----- Store IR nodes ----- */
StoreIrNode::StoreIrNode(string type, string op1, string res):
    IrNode("STORE"), type(type), op1(op1), res(res) {
    if(!isLiteral(op1)) genSet.insert(op1);
    if(!isLiteral(res)) killSet.insert(res);
}

StoreIrNode::~StoreIrNode() {}

stringstream StoreIrNode::print() {
    stringstream ss = IrNode::print();
    ss << type << " " << op1 << " " << res;
    return ss;
}

/* ----- Read IR nodes ----- */
ReadIrNode::ReadIrNode(string type, string res):
    IrNode("READ"), type(type), res(res) {
    if(!isLiteral(res)) killSet.insert(res);
}

ReadIrNode::~ReadIrNode() {}

stringstream ReadIrNode::print() {
    stringstream ss = IrNode::print();
    ss << type << " " << res;
    return ss;
}

/* ----- Write IR nodes ----- */
WriteIrNode::WriteIrNode(string type, string op1):
    IrNode("WRITE"), type(type), op1(op1) {
    if(!isLiteral(op1)) genSet.insert(op1);
}

WriteIrNode::~WriteIrNode() {}

stringstream WriteIrNode::print() {
    stringstream ss = IrNode::print();
    ss << cmd << type << " " << op1;
    return ss;
}

/* ----- Call IR nodes ----- */
CallIrNode::CallIrNode(string name):
    IrNode("JSR"), name(name) {}

CallIrNode::~CallIrNode() {}

stringstream CallIrNode::print() {
    stringstream ss = IrNode::print();
    ss << " " << "FUNC_" << name;
    return ss;
}

/* ----- Push IR nodes ----- */
PushIrNode::PushIrNode():
    IrNode("PUSH"), op1("") {}

PushIrNode::PushIrNode(string op1):
    IrNode("PUSH"), op1(op1) {
    if(!isLiteral(op1)) genSet.insert(op1);
}

PushIrNode::~PushIrNode() {}

stringstream PushIrNode::print() {
    stringstream ss = IrNode::print();
    if(op1 == "!REGS") ss << cmd << "REGS";
    else ss << cmd << " " << op1;
    return ss;
}

/* ----- Pop IR nodes ----- */
PopIrNode::PopIrNode():
    IrNode("POP"), op1("") {}

PopIrNode::PopIrNode(string op1):
    IrNode("POP"), op1(op1) {
    if(!isLiteral(op1)) killSet.insert(op1);
}

PopIrNode::~PopIrNode() {}

stringstream PopIrNode::print() {
    stringstream ss = IrNode::print();
    if(op1 == "!REGS") ss << cmd << "REGS";
    else ss << cmd << " " << op1;
    return ss;
}

/* ----- Jump IR nodes ----- */
JumpIrNode::JumpIrNode(string label):
    IrNode("JUMP"), label(label) {}

JumpIrNode::~JumpIrNode() {}

stringstream JumpIrNode::print() {
    stringstream ss = IrNode::print();
    ss << " " << label;
    return ss;
}

/* ----- Link IR nodes ----- */
LinkIrNode::LinkIrNode(int size):
    IrNode("LINK"), size(size) {}

LinkIrNode::~LinkIrNode() {}

stringstream LinkIrNode::print() {
    stringstream ss = IrNode::print();
    ss << " " << size;
    return ss;
}


void IrNode::livenessAna() {
    // calculate in/out set
    reverse(worklist.begin(),worklist.end());   // reverse the worklist
    while(!worklist.empty()){ 
        IrNode* lastNode = worklist.front();
        worklist.pop_front();                    
        lastNode->livenessCalc(); 
    }
}
