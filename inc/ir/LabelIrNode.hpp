#ifndef LABELIRNODE_HPP_
#define LABELIRNODE_HPP_

#include "irNode.hpp"

class LabelIrNode: public IrNode {
    IrNode* predecessor2;
    std::string label;
public:
    LabelIrNode(std::string, regManager&);
    virtual ~LabelIrNode();
    virtual std::stringstream print();
    virtual void setPre2(IrNode*);
    virtual void updateWorklist();

    virtual std::vector<std::string> translate();       // translate into opcode
};

#endif
