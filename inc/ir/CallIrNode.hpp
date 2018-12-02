#ifndef CALLIRNODE_HPP_
#define CALLIRNODE_HPP_

#include "irNode.hpp"

class CallIrNode: public IrNode {
    std::string name;
public:
    CallIrNode(std::string, regManager&);
    virtual ~CallIrNode();
    virtual std::stringstream print();
    virtual std::vector<std::string> translate();       // translate into opcode
};

#endif
