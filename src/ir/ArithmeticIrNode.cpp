#include "../../inc/irNode.hpp"
#include "../../inc/utility.hpp"
#include <assert.h>

using namespace std;

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

    // int regX = regMan.regEnsure(op1, -1, opCodeBlock, outSet);
    // int regY = regMan.regEnsure(op2, regX, opCodeBlock, outSet);
    
    int regX, regY;
    regMan.regEnsure2(op1, op2, regX, regY, opCodeBlock, outSet);

    assert(regX != regY || op1 == op2);
    if(outSet.find(op1) == outSet.end()) regMan.regFree(regX, opCodeBlock, outSet);
    int regZ = regMan.regAllocate(res, regY, opCodeBlock, outSet);

    if(outSet.find(op2) == outSet.end()) regMan.regFree(regY, opCodeBlock, outSet);
    assert(regZ != regY);
    if(regZ != regX) {
        opCodeBlock.push_back("move r" +  to_string(regX) + " r" + to_string(regZ));
    }
    opCodeBlock.push_back(toLower(cmd+type) + " r" +  to_string(regY) + " r" + to_string(regZ));
    regMan.markDirty(regZ);
    
    return opCodeBlock;
}
