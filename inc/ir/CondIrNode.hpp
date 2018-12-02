#ifndef CONDIRNODE_HPP_
#define CONDIRNODE_HPP_

#include "irNode.hpp"

class CondIrNode: public IrNode {
    IrNode* successor2; 
    std::string cond;
    std::string type;
    std::string op1;
    std::string op2;
    std::string jumpTo;
public:
    CondIrNode(std::string, std::string, std::string, 
            std::string, std::string, regManager&);
    virtual ~CondIrNode();
    virtual std::stringstream print();
    virtual void setSuc2(IrNode*);
    virtual bool livenessCalc();
    
    virtual std::vector<std::string> translate();       // translate into opcode
};

#endif
