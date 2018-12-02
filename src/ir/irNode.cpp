#include "../../inc/ir/irNode.hpp"
#include "../../inc/ir/WriteIrNode.hpp"
#include "../../inc/ir/ReadIrNode.hpp"
#include "../../inc/ir/StoreIrNode.hpp"
#include "../../inc/ir/ArithmeticIrNode.hpp"
#include "../../inc/utility.hpp"
#include <assert.h>
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
    cout << left << setfill(' ') << setw(40) << print().str();
    cout << left << setfill(' ') << setw(30) << printIn().str();
    cout << left << setfill(' ') << setw(30) << printOut().str();
}

void IrNode::setPre(IrNode* predecessor) { this->predecessor = predecessor; }

void IrNode::setSuc(IrNode* successor) { this->successor = successor; }

void IrNode::insertOutSet(string id) { if(!isLiteral(id)) outSet.insert(id); }

void IrNode::insertInSet(string id) { if(!isLiteral(id)) inSet.insert(id); }

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

stringstream IrNode::printGen() {
    stringstream ss;
    ss << "Gen SET: ";
    for(auto id: inSet){
        ss << id << " ";
    }
    return ss;
}

stringstream IrNode::printKill() {
    stringstream ss;
    ss << "Kill SET: ";
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
        regMan.pushAll();
        opCodeBlock.push_back("push r0");
        opCodeBlock.push_back("push r1");
        opCodeBlock.push_back("push r2");
        opCodeBlock.push_back("push r3");
    }
    else if(cmd == "POPREGS") {
        regMan.popAll();
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


void IrNode::constant_swap(vector<IrNode*> irs) {

    unordered_map<string, string> const_refs;

    for(auto ir = irs.begin(); ir != irs.end();)
    {
        string cmd = (*ir)->cmd;
        if(cmd == "STORE") {
            StoreIrNode* storeIr = static_cast<StoreIrNode*>(*ir);
            if(is_constant(storeIr->op1)) {            // op1 is a literal
                const_refs[storeIr->res] = storeIr->op1;
                ir = irs.erase(ir);
            }
            else if(const_refs.find(storeIr->op1) != const_refs.end()) {// or const ref
                const_refs[storeIr->res] = const_refs[storeIr->op1];
                ir = irs.erase(ir);
            }
            else {
                const_refs.erase(storeIr->res);    
                ir++;
            }
        }
        else if(cmd == "READ"){
            ReadIrNode* readIr = static_cast<ReadIrNode*>(*ir);
            const_refs.erase(readIr->res);    // not a constant anymore
            ir++;
        }
        else if(cmd == "MUL" || cmd == "ADD" || cmd == "DIV" || cmd == "SUB")
        {
            ArithmeticIrNode* arithIr = static_cast<ArithmeticIrNode*>(*ir);
            
            if((is_constant(arithIr->op1) || const_refs.find(arithIr->op1) != const_refs.end()) && // op1 is a literal or const ref
                (is_constant(arithIr->op2) || const_refs.find(arithIr->op2) != const_refs.end())) // and op2 is a literal or const ref
            {
                // op1 and op2 are constant
                if(arithIr->type == "I"){
                    int op1 = is_constant(arithIr->op1)?stoi(arithIr->op1):stoi(const_refs[arithIr->op1]);
                    int op2 = is_constant(arithIr->op2)?stoi(arithIr->op2):stoi(const_refs[arithIr->op2]);
                    int res;
                    if(arithIr->cmd == "MUL") res = op1*op2;
                    if(arithIr->cmd == "ADD") res = op1+op2;
                    if(arithIr->cmd == "DIV") res = op1/op2;
                    if(arithIr->cmd == "SUB") res = op1-op2;
                    const_refs[arithIr->res] = to_string(res);
                }
                else{
                    double op1 = is_constant(arithIr->op1)?stof(arithIr->op1):stof(const_refs[arithIr->op1]);
                    double op2 = is_constant(arithIr->op2)?stof(arithIr->op2):stof(const_refs[arithIr->op2]);
                    double res;
                    if(arithIr->cmd == "MUL") res = op1*op2;
                    if(arithIr->cmd == "ADD") res = op1+op2;
                    if(arithIr->cmd == "DIV") res = op1/op2;
                    if(arithIr->cmd == "SUB") res = op1-op2;
                    const_refs[arithIr->res] = to_string(res);
                }
                ir = irs.erase(ir);
            }
            else{
                if(const_refs.find(arithIr->op1) != const_refs.end())
                    arithIr->op1 = const_refs[arithIr->op1];
                if(const_refs.find(arithIr->op2) != const_refs.end())
                    arithIr->op2 = const_refs[arithIr->op2];
                const_refs.erase(arithIr->res);
                ir++;
            }
        }
        else if(cmd == "WRITE") {
            WriteIrNode* writeIr = static_cast<WriteIrNode*>(*ir);
            if(const_refs.find(writeIr->op1) != const_refs.end()){
                //store back to register for priting
                StoreIrNode* storeIr = new StoreIrNode(writeIr->type, const_refs[writeIr->op1],
                        "!T-1", writeIr->regMan);
                writeIr->op1 = "!T-1";
                irs.insert(ir,storeIr);
                ir++;
            }
            ir++;
        }
        else if(cmd == "LABEL" || cmd == "END") {
            // this is another block,
            // spill everything in the buffer
            
            for(auto kv: const_refs) {
                string type = "I";      // for now
                                        // TODO: change to actual type
                ir = irs.insert(ir, new StoreIrNode(type, kv.first, kv.second, (*ir)->regMan));
            }

            const_refs.clear();
        }
        else{
            ir++;
        }
    }
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

