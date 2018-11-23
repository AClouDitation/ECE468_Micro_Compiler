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
    cout << left << setfill(' ') << setw(20) << print().str();
    cout << left << setfill(' ') << setw(30) << printIn().str();
    cout << left << setfill(' ') << setw(30) << printOut().str();
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

vector<string> IrNode::translate() {
    vector<string> opCodeBlock;

    if(cmd == "PUSHREGS") {
        opCodeBlock.push_back("push r0");
        opCodeBlock.push_back("push r1");
        opCodeBlock.push_back("push r2");
        opCodeBlock.push_back("push r3");
    }
    else if(cmd == "POPREGS") {
        opCodeBlock.push_back("pop r3");
        opCodeBlock.push_back("pop r2");
        opCodeBlock.push_back("pop r1");
        opCodeBlock.push_back("pop r0");
    }
    else {
        opCodeBlock.push_back(toLower(cmd));
    }
    return opCodeBlock;
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

vector<string> ArithmeticIrNode::translate() {

    vector<string> opCodeBlock;
    /* register allocation */

    int regX = regMan.regEnsure(op1, -1, opCodeBlock, outSet);
    if(outSet.find(op1) == outSet.end()) regMan.regFree(regX, opCodeBlock, outSet);
    int regZ = regMan.regAllocate(res, -1, opCodeBlock, outSet);
    if(regX != regZ) opCodeBlock.push_back("move r" + to_string(regX) + " r" + to_string(regZ));

    if(op2 == op1) {    // since op1 has been freed, it might cause some trouble
        opCodeBlock.push_back(toLower(cmd+type) + " r" +  to_string(regZ) + " r" + to_string(regZ));
    }
    else {
        int regY = regMan.regEnsure(op2, regZ, opCodeBlock, outSet);
        if(outSet.find(op2) == outSet.end()) regMan.regFree(regY, opCodeBlock, outSet);
        //cout << regMan.print().str() << endl;
        opCodeBlock.push_back(toLower(cmd+type) + " r" +  to_string(regY) + " r" + to_string(regZ));
    }

    regMan.markDirty(regZ);
    
    return opCodeBlock;
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

vector<string> StoreIrNode::translate() {
    vector<string> opCodeBlock;
    int regX = regMan.regEnsure(op1, -1, opCodeBlock, outSet);
    if(outSet.find(op1) == outSet.end()) regMan.regFree(regX, opCodeBlock, outSet);
    int regZ = regMan.regAllocate(res, -1, opCodeBlock, outSet);
    regMan.markDirty(regZ);

    if(regX != regZ)
        opCodeBlock.push_back("move r" + to_string(regX) + " r" + to_string(regZ));   // TODO: this seems redundant...
                                                // optimize it
    return opCodeBlock;
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

vector<string> ReadIrNode::translate() {
    vector<string> opCodeBlock;
    int regZ = regMan.regAllocate(res, -1, opCodeBlock, outSet);
    regMan.markDirty(regZ);

    opCodeBlock.push_back("sys " + toLower(cmd+type) + " r" + to_string(regZ));
    return opCodeBlock;
}

/* ----- Write IR nodes ----- */
WriteIrNode::WriteIrNode(string type, string op1, regManager& regMan):
    IrNode("WRITE", regMan), type(type), op1(op1) {
    if(!isLiteral(op1) && type != "S") genSet.insert(op1);
}

WriteIrNode::~WriteIrNode() {}

stringstream WriteIrNode::print() {
    stringstream ss = IrNode::print();
    ss << type << " " << op1;
    return ss;
}

vector<string> WriteIrNode::translate() {
    vector<string> opCodeBlock;
    if(type == "S") {
        opCodeBlock.push_back("sys " + toLower(cmd+type)+ " " + op1);
    }
    else {
        int regX = regMan.regEnsure(op1, -1, opCodeBlock, outSet);
        if(outSet.find(op1) == outSet.end()) regMan.regFree(regX, opCodeBlock, outSet);

        opCodeBlock.push_back("sys " + toLower(cmd+type) + " r" + to_string(regX));
    }
    return opCodeBlock;
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

vector<string> CallIrNode::translate() {
    vector<string> opCodeBlock;
    opCodeBlock.push_back("jsr FUNC_" + name);
    return opCodeBlock;
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

vector<string> PushIrNode::translate() {
    vector<string> opCodeBlock;
    if(op1 == "") {
        opCodeBlock.push_back("push");
        return opCodeBlock;
    }

    int regX = regMan.regEnsure(op1, -1, opCodeBlock, outSet);
    if(outSet.find(op1) == outSet.end()) regMan.regFree(regX, opCodeBlock, outSet);
    opCodeBlock.push_back("push r" + to_string(regX));
    return opCodeBlock;
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

vector<string> PopIrNode::translate() {
    vector<string> opCodeBlock;
    if(op1 == "") {
        opCodeBlock.push_back("push");
        return opCodeBlock;
    }
    int regZ = regMan.regAllocate(op1, -1, opCodeBlock, outSet);
    regMan.markDirty(regZ);
    opCodeBlock.push_back("pop r" + to_string(regZ));
    return opCodeBlock;
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

vector<string> JumpIrNode::translate() {
    vector<string> opCodeBlock;
    opCodeBlock.push_back("jmp " + label);
    return opCodeBlock;
}

/* ----- Retuen IR nodes ----- */
ReturnIrNode::ReturnIrNode(int retLoc, regManager& regMan):
    IrNode("RET", regMan), retLoc(retLoc) {}

ReturnIrNode::~ReturnIrNode() {}

vector<string> ReturnIrNode::translate() {
    vector<string> opCodeBlock;
    // free all global var
    regMan.freeGlobal(opCodeBlock);
    // free return value
    regMan.freeReturn(opCodeBlock, retLoc);

    return opCodeBlock;
}
