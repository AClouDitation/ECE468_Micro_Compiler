#ifndef ARITHMETICIRNODE_HPP_
#define ARITHMETICIRNODE_HPP_

#include "irNode.hpp"

class ArithmeticIrNode: public IrNode {
    std::string type;
    std::string op1;
    std::string op2;
    std::string res;
public:
    ArithmeticIrNode(std::string, std::string, 
            std::string, std::string, std::string, regManager&);
    virtual ~ArithmeticIrNode();
    virtual std::stringstream print();
    
    virtual std::vector<std::string> translate();       // translate into opcode
    friend void IrNode::constant_swap(std::vector<IrNode*>);
};

#endif
