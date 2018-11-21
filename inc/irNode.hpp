#ifndef IRNODE_HPP_
#define IRNODE_HPP_

#include <string>
#include <list>
#include <set>
#include <sstream>

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
    IrNode(std::string);
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
    static void livenessAna();
    friend class CondIrNode;       // don't know why...
};

class ArithmeticIrNode: public IrNode {
    std::string type;
    std::string op1;
    std::string op2;
    std::string res;
public:
    ArithmeticIrNode(std::string, std::string, 
            std::string, std::string, std::string);
    virtual ~ArithmeticIrNode();
    virtual std::stringstream print();
};

class StoreIrNode: public IrNode {
    std::string type;
    std::string op1;
    std::string res;
public:
    StoreIrNode(std::string, std::string, std::string);
    virtual ~StoreIrNode();
    virtual std::stringstream print();
};

class ReadIrNode: public IrNode {
    std::string type;
    std::string res;
public:
    ReadIrNode(std::string, std::string);
    virtual ~ReadIrNode();
    virtual std::stringstream print();
};

class WriteIrNode: public IrNode {
    std::string type;
    std::string op1;
public:
    WriteIrNode(std::string, std::string);
    virtual ~WriteIrNode();
    virtual std::stringstream print();
};


class CallIrNode: public IrNode {
    std::string name;
public:
    CallIrNode(std::string);
    virtual ~CallIrNode();
    virtual std::stringstream print();
};

class PushIrNode: public IrNode {
    std::string op1;
public:
    PushIrNode();
    PushIrNode(std::string);
    virtual ~PushIrNode();
    virtual std::stringstream print();
};

class PopIrNode: public IrNode {
    std::string op1;
public:
    PopIrNode();
    PopIrNode(std::string);
    virtual ~PopIrNode();
    virtual std::stringstream print();
};

class JumpIrNode: public IrNode {
    std::string label;
public:
    JumpIrNode(std::string);
    virtual ~JumpIrNode();
    virtual std::stringstream print();
};

class LinkIrNode: public IrNode {
    int size;
public:
    LinkIrNode(int);
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
            std::string, std::string);
    virtual ~CondIrNode();
    virtual std::stringstream print();
    virtual void setSuc2(IrNode*);
    virtual bool livenessCalc();
};

class LabelIrNode: public IrNode {
    IrNode* predecessor2;
    std::string label;
public:
    LabelIrNode(std::string);
    virtual ~LabelIrNode();
    virtual std::stringstream print();
    virtual void setPre2(IrNode*);
};
#endif //IRNODE_HPP_
