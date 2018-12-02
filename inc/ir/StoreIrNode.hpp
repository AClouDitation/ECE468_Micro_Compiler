#ifndef STOREIRNODE_HPP_
#define STOREIRNODE_HPP_

#include "irNode.hpp"

class StoreIrNode: public IrNode {
    std::string type;
    std::string op1;
    std::string res;
public:
    StoreIrNode(std::string, std::string, std::string, regManager&);
    virtual ~StoreIrNode();
    virtual std::stringstream print();

    virtual std::vector<std::string> translate();
    friend void IrNode::constant_swap(std::vector<IrNode*>);
};

#endif
