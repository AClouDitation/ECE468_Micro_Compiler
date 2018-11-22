#ifndef IRNODE_HPP_
#define IRNODE_HPP_

#include <string>
#include <list>
#include <set>
#include <vector>
#include <sstream>
#include "regman.hpp"

class IrNode {
protected:
    std::string cmd;
    IrNode* predecessor;
    IrNode* successor;
    std::set<std::string> genSet;
    std::set<std::string> killSet;
    std::set<std::string> inSet;
    std::set<std::string> outSet;
    regManager& regMan;

    static std::list<IrNode*> worklist;

public:
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
    virtual bool livenessCalc();    // return true if inSet got updated
    virtual void updateWorklist();  // put predecessor into worklist
    virtual void regAlloc();        // do nothing since base class IrNode does not have any oprand
    
    static void livenessAna();
    friend class CondIrNode;       // don't know why...
    friend void irBlockCascade(std::vector<IrNode*>& block, std::vector<IrNode*>& newBlock);    // TODO: make this a member function
    friend void irBlockInsert(std::vector<IrNode*>& block, IrNode* newIr);                           // and this
};

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
    virtual void regAlloc();
};

class StoreIrNode: public IrNode {
    std::string type;
    std::string op1;
    std::string res;
public:
    StoreIrNode(std::string, std::string, std::string, regManager&);
    virtual ~StoreIrNode();
    virtual std::stringstream print();
    virtual void regAlloc();
};

class ReadIrNode: public IrNode {
    std::string type;
    std::string res;
public:
    ReadIrNode(std::string, std::string, regManager&);
    virtual ~ReadIrNode();
    virtual std::stringstream print();
    virtual void regAlloc();
};

class WriteIrNode: public IrNode {
    std::string type;
    std::string op1;
public:
    WriteIrNode(std::string, std::string, regManager&);
    virtual ~WriteIrNode();
    virtual std::stringstream print();
    virtual void regAlloc();
};


class CallIrNode: public IrNode {
    std::string name;
public:
    CallIrNode(std::string, regManager&);
    virtual ~CallIrNode();
    virtual std::stringstream print();
};

class PushIrNode: public IrNode {
    std::string op1;
public:
    PushIrNode(regManager&);
    PushIrNode(std::string, regManager&);
    virtual ~PushIrNode();
    virtual std::stringstream print();
    virtual void regAlloc();
};

class PopIrNode: public IrNode {
    std::string op1;
public:
    PopIrNode(regManager&);
    PopIrNode(std::string, regManager&);
    virtual ~PopIrNode();
    virtual std::stringstream print();
    virtual void regAlloc();
};

class JumpIrNode: public IrNode {
    std::string label;
public:
    JumpIrNode(std::string, regManager&);
    virtual ~JumpIrNode();
    virtual std::stringstream print();
};

class LinkIrNode: public IrNode {
    int size;
public:
    LinkIrNode(int, regManager&);
    virtual ~LinkIrNode();
    virtual std::stringstream print();
};

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
    virtual void regAlloc();
};

class LabelIrNode: public IrNode {
    IrNode* predecessor2;
    std::string label;
public:
    LabelIrNode(std::string, regManager&);
    virtual ~LabelIrNode();
    virtual std::stringstream print();
    virtual void setPre2(IrNode*);
    virtual void updateWorklist();
};
#endif //IRNODE_HPP_
