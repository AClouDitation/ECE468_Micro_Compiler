#ifndef RETURNIRNODE_HPP_
#define RETURNIRNODE_HPP_

#include "irNode.hpp"

class ReturnIrNode: public IrNode {
    int retLoc;
public:
    ReturnIrNode(int, regManager&);
    virtual ~ReturnIrNode();
    
    virtual std::vector<std::string> translate();       // translate into opcode
};

#endif
