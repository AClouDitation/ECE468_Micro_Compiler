#ifndef IRNODE_HPP_
#define IRNODE_HPP_

#include <string>
#include <list>
#include <set>
#include <vector>
#include <sstream>
#include "../regman.hpp"

class IrNode {
protected:
    std::string cmd;
    IrNode* predecessor;
    IrNode* successor;
    std::set<std::string> genSet;
    std::set<std::string> killSet;
    std::set<std::string> inSet;
    std::set<std::string> outSet;

    static std::list<IrNode*> worklist;

public:
    regManager& regMan;     // FIXME: put this into protected
    IrNode(std::string, regManager&);
    virtual ~IrNode();
    virtual std::stringstream print();
    virtual void reformatPrint();
    virtual void setPre(IrNode*);
    virtual void setSuc(IrNode*);
    virtual void insertOutSet(std::string);
    virtual void insertInSet(std::string);

    virtual std::stringstream printIn();
    virtual std::stringstream printOut();
    virtual std::stringstream printGen();
    virtual std::stringstream printKill();
    virtual bool livenessCalc();    // return true if inSet got updated
    virtual void updateWorklist();  // put predecessor into worklist

    virtual std::vector<std::string> translate();       // translate into opcode
    
    static void constant_swap(std::vector<IrNode*>);
    static void livenessAna();

    friend class CondIrNode;       // don't know why...
    friend void irBlockCascade(std::vector<IrNode*>& block, std::vector<IrNode*>& newBlock);    // TODO: make this a member function
    friend void irBlockInsert(std::vector<IrNode*>& block, IrNode* newIr);                           // and this
};

#endif //IRNODE_HPP_
