#ifndef READIRNODE_HPP_
#define READIRNODE_HPP_

#include "irNode.hpp"

class ReadIrNode: public IrNode {
    std::string type;
    std::string res;
public:
    ReadIrNode(std::string, std::string, regManager&);
    virtual ~ReadIrNode();
    virtual std::stringstream print();
    
    virtual std::vector<std::string> translate();       // translate into opcode
    friend void IrNode::constant_swap(std::vector<IrNode*>);
};

#endif
