#ifndef JUMPIRNODE_HPP_
#define JUMPIRNODE_HPP_

#include "irNode.hpp"

class JumpIrNode: public IrNode {
    std::string label;
public:
    JumpIrNode(std::string, regManager&);
    virtual ~JumpIrNode();
    virtual std::stringstream print();
    virtual std::vector<std::string> translate();       // translate into opcode
};

#endif
