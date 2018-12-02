#ifndef WRITEIRNODE_HPP_
#define WRITEIRNODE_HPP_

#include "irNode.hpp"

class WriteIrNode: public IrNode {
    std::string type;
    std::string op1;
public:
    WriteIrNode(std::string, std::string, regManager&);
    virtual ~WriteIrNode();
    virtual std::stringstream print();
    
    virtual std::vector<std::string> translate();       // translate into opcode
    friend void IrNode::constant_swap(std::vector<IrNode*>);
};

#endif
