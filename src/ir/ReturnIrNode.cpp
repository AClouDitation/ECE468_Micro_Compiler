#include "../../inc/irNode.hpp"
#include "../../inc/utility.hpp"
#include <assert.h>

using namespace std;

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

    opCodeBlock.push_back("unlnk");
    opCodeBlock.push_back("ret");
    return opCodeBlock;
}
