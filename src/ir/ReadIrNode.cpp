#include "../../inc/ir/ReadIrNode.hpp"
#include "../../inc/utility.hpp"
#include <assert.h>

using namespace std;

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

    if(outSet.find(res) == outSet.end()) regMan.regFree(regZ, opCodeBlock, outSet);
    return opCodeBlock;
}
