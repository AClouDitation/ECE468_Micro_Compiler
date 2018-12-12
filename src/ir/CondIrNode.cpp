#include "../../inc/ir/CondIrNode.hpp"
#include "../../inc/utility.hpp"

using namespace std;

/* ----- conditional IR nodes ----- */
CondIrNode::CondIrNode(string cond, string type,
    string op1, string op2, string jumpTo, regManager& regMan):
    IrNode(cond, regMan), successor2(NULL), cond(cond), type(type), 
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
    if(successor2) for(auto id: successor2->inSet) outSet.insert(id);
    set<string> inSetbk = inSet;                // backup inSet

    inSet = outSet;                             // initialize inSet

    /* calculate new inSet */
    if(successor2) for(auto id: successor2->inSet) inSet.insert(id);
    for(auto id: killSet) inSet.erase(id);
    for(auto id: genSet)  inSet.insert(id);

    return inSet != inSetbk;
}

vector<string> CondIrNode::translate() {
    vector<string> opCodeBlock;

    if(cmd == "FALSE" || cmd == "TRUE") {
        regMan.regFree(0, opCodeBlock, outSet);
        regMan.regFree(1, opCodeBlock, outSet);
        regMan.regFree(2, opCodeBlock, outSet);
        regMan.regFree(3, opCodeBlock, outSet);

        if(cmd == "FALSE") {
            opCodeBlock.push_back("jmp " + jumpTo);
        }
        return opCodeBlock;
    }

    int regX, regY;
    regMan.regEnsure2(op1, op2, regX, regY, opCodeBlock, outSet);

    if(outSet.find(op1) == outSet.end()) regMan.regFree(regX, opCodeBlock, outSet);
    if(outSet.find(op2) == outSet.end()) regMan.regFree(regY, opCodeBlock, outSet);

    // spill everything before jump
    regMan.regFree(0, opCodeBlock, outSet);
    regMan.regFree(1, opCodeBlock, outSet);
    regMan.regFree(2, opCodeBlock, outSet);
    regMan.regFree(3, opCodeBlock, outSet);

    opCodeBlock.push_back("cmp" + toLower(type) + " r" + to_string(regX)  + " r" + to_string(regY));
    opCodeBlock.push_back("j" + toLower(cmd) + " " + jumpTo);


    return opCodeBlock;
}
