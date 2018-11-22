#include "../../inc/irNode.hpp"
#include "../../inc/utility.hpp"
#include <algorithm>
#include <iomanip>

using namespace std;

list<IrNode*> IrNode::worklist;

/* ----- generic IR nodes ----- */
IrNode::IrNode(string cmd, regManager& regMan):
    cmd(cmd), predecessor(NULL), successor(NULL), regMan(regMan){
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

void IrNode::updateWorklist() {
    if(!predecessor) return;
    for(auto irNode: worklist) if(irNode == predecessor) return;
    worklist.push_back(predecessor);
    predecessor->updateWorklist();
}

void IrNode::regAlloc() {}

/* ----- Arithmetic IR nodes ----- */
ArithmeticIrNode::ArithmeticIrNode(string cmd, string type, string op1, 
    string op2, string res, regManager& regMan):
    IrNode(cmd, regMan), type(type), op1(op1), op2(op2), res(res) {
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

void ArithmeticIrNode::regAlloc() {
    int regX = regMan.regEnsure(op1);
    int regY = regMan.regEnsure(op2);
    if(outSet.find(op1) == outSet.end()) regMan.regFree(regX);
    if(outSet.find(op2) == outSet.end()) regMan.regFree(regY);
    int regZ = regMan.regAllocate(res);
    regMan.markDirty(regZ);
}

/* ----- Store IR nodes ----- */
StoreIrNode::StoreIrNode(string type, string op1, 
        string res, regManager& regMan):
    IrNode("STORE", regMan), type(type), op1(op1), res(res) {
    if(!isLiteral(op1)) genSet.insert(op1);
    if(!isLiteral(res)) killSet.insert(res);
}

StoreIrNode::~StoreIrNode() {}

stringstream StoreIrNode::print() {
    stringstream ss = IrNode::print();
    ss << type << " " << op1 << " " << res;
    return ss;
}

void StoreIrNode::regAlloc() {
    int regX = regMan.regEnsure(op1);
    if(outSet.find(op1) == outSet.end()) regMan.regFree(regX);
    int regZ = regMan.regAllocate(res);
    regMan.markDirty(regZ);
}

/* ----- Read IR nodes ----- */
ReadIrNode::ReadIrNode(string type, string res, regManager& regMan):
    IrNode("READ", regMan), type(type), res(res) {
    if(!isLiteral(res)) killSet.insert(res);
}

ReadIrNode::~ReadIrNode() {}

stringstream ReadIrNode::print() {
    stringstream ss = IrNode::print();
    ss << type << " " << res;
    return ss;
}

void ReadIrNode::regAlloc() {
    int regZ = regMan.regAllocate(res);
    regMan.markDirty(regZ);
}
/* ----- Write IR nodes ----- */
WriteIrNode::WriteIrNode(string type, string op1, regManager& regMan):
    IrNode("WRITE", regMan), type(type), op1(op1) {
    if(!isLiteral(op1)) genSet.insert(op1);
}

WriteIrNode::~WriteIrNode() {}

stringstream WriteIrNode::print() {
    stringstream ss = IrNode::print();
    ss << type << " " << op1;
    return ss;
}

void WriteIrNode::regAlloc() {
    int regX = regMan.regEnsure(op1);
    if(outSet.find(op1) == outSet.end()) regMan.regFree(regX);
}

/* ----- Call IR nodes ----- */
CallIrNode::CallIrNode(string name, regManager& regMan):
    IrNode("JSR", regMan), name(name) {}

CallIrNode::~CallIrNode() {}

stringstream CallIrNode::print() {
    stringstream ss = IrNode::print();
    ss << " " << "FUNC_" << name;
    return ss;
}

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

void PushIrNode::regAlloc() {
    if(op1 == "") return;
    int regX = regMan.regEnsure(op1);
    if(outSet.find(op1) == outSet.end()) regMan.regFree(regX);
}

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

void PopIrNode::regAlloc() {
    int regZ = regMan.regAllocate(op1);
    regMan.markDirty(regZ);
}

/* ----- Jump IR nodes ----- */
JumpIrNode::JumpIrNode(string label, regManager& regMan):
    IrNode("JUMP", regMan), label(label) {}

JumpIrNode::~JumpIrNode() {}

stringstream JumpIrNode::print() {
    stringstream ss = IrNode::print();
    ss << " " << label;
    return ss;
}

/* ----- Link IR nodes ----- */
LinkIrNode::LinkIrNode(int size, regManager& regMan):
    IrNode("LINK", regMan), size(size) {}

LinkIrNode::~LinkIrNode() {}

stringstream LinkIrNode::print() {
    stringstream ss = IrNode::print();
    ss << " " << size;
    return ss;
}

void IrNode::livenessAna() {
    reverse(worklist.begin(),worklist.end());   // reverse the worklist
    while(!worklist.empty()){ 
        IrNode* lastNode = worklist.front();
        worklist.pop_front();                    
        // push all predecessor of this node into worklist
        if(lastNode->livenessCalc()) lastNode->updateWorklist();            
    }
}
