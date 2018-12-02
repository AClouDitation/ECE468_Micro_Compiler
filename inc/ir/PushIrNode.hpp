#ifndef PUSHIRNODE_HPP_
#define PUSHIRNODE_HPP_

#include "irNode.hpp"

class PushIrNode: public IrNode {
    std::string op1;
public:
    PushIrNode(regManager&);
    PushIrNode(std::string, regManager&);
    virtual ~PushIrNode();
    virtual std::stringstream print();
    
    virtual std::vector<std::string> translate();       // translate into opcode
};

#endif
