#include "../../inc/irNode.hpp"
#include "../../inc/utility.hpp"
#include <assert.h>

using namespace std;

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

