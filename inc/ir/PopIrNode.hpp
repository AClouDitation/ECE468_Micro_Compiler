#ifndef POPIRNODE_HPP_
#define POPIRNODE_HPP_

#include "irNode.hpp"

class PopIrNode: public IrNode {
    std::string op1;
public:
    PopIrNode(regManager&);
    PopIrNode(std::string, regManager&);
    virtual ~PopIrNode();
    virtual std::stringstream print();
    
    virtual std::vector<std::string> translate();       // translate into opcode
};

#endif
