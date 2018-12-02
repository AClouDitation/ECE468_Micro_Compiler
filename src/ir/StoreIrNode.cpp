#include "../../inc/irNode.hpp"
#include "../../inc/utility.hpp"
#include <assert.h>

using namespace std;

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

