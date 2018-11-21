#ifndef IRNODE_HPP_
#define IRNODE_HPP_

#include <string>
#include <queue>
#include <unordered_set>

class IrNode {
protected:
    std::string cmd;
    IrNode* predecessor;
    IrNode* successor;
    std::unordered_set<std::string> genSet;
    std::unordered_set<std::string> killSet;
    std::unordered_set<std::string> inSet;
    std::unordered_set<std::string> outSet;

    static std::queue<IrNode*> worklist;

public:
    IrNode(std::string);
    virtual ~IrNode();
    virtual void print();
    virtual void setPre(IrNode*);
    virtual void setSuc(IrNode*);
    virtual void insertOutSet(std::string);
    virtual void insertInSet(std::string);

    virtual void printGen();
    virtual void printKill();
    virtual void livenessCalc();
    friend class splitIrNode;   // don't know why...
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
    virtual void print();
};

class StoreIrNode: public IrNode {
    std::string type;
    std::string op1;
    std::string res;
public:
    StoreIrNode(std::string, std::string, std::string);
    virtual ~StoreIrNode();
    virtual void print();
};

class ReadIrNode: public IrNode {
    std::string type;
    std::string res;
public:
    ReadIrNode(std::string, std::string);
    virtual ~ReadIrNode();
    virtual void print();
};

class WriteIrNode: public IrNode {
    std::string type;
    std::string op1;
public:
    WriteIrNode(std::string, std::string);
    virtual ~WriteIrNode();
    virtual void print();
};


class CallIrNode: public IrNode {
    std::string name;
public:
    CallIrNode(std::string);
    virtual ~CallIrNode();
    virtual void print();
};

class PushIrNode: public IrNode {
    std::string op1;
public:
    PushIrNode();
    PushIrNode(std::string);
    virtual ~PushIrNode();
    virtual void print();
};

class PopIrNode: public IrNode {
    std::string op1;
public:
    PopIrNode();
    PopIrNode(std::string);
    virtual ~PopIrNode();
    virtual void print();
};

class JumpIrNode: public IrNode {
    std::string label;
public:
    JumpIrNode(std::string);
    virtual ~JumpIrNode();
    virtual void print();
};

class LinkIrNode: public IrNode {
    int size;
public:
    LinkIrNode(int);
    virtual ~LinkIrNode();
    virtual void print();
};

class splitIrNode: public IrNode {
    IrNode* successor2; 
public:
    splitIrNode(std::string);
    virtual ~splitIrNode();
    virtual void print();
    virtual void setSuc2(IrNode*);
    virtual void livenessCalc();
};

class mergeIrNode: public IrNode {
    IrNode* predecessor2;
public:
    mergeIrNode(std::string);
    virtual ~mergeIrNode();
    virtual void print();
    virtual void setPre2(IrNode*);
};  // could be multiple merge,
    // only support two-branch merging for now

class CondIrNode: public splitIrNode {
    std::string cond;
    std::string type;
    std::string op1;
    std::string op2;
    std::string jumpTo;
public:
    CondIrNode(std::string, std::string, std::string, 
            std::string, std::string);
    virtual ~CondIrNode();
    virtual void print();
};

class LabelIrNode: public mergeIrNode {
    std::string label;
public:
    LabelIrNode(std::string);
    virtual ~LabelIrNode();
    virtual void print();
};
#endif //IRNODE_HPP_
