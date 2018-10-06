#ifndef AST_HPP
#define AST_HPP
#include <string>
#include <vector>

// expressions

static int temp_reg_index = 0;
class ExprNode{
public:
    ExprNode(){};
    virtual ~ExprNode(){};
    virtual std::string translate(std::vector<std::string>&)=0;
    ExprNode* lnode;
    ExprNode* rnode;
};

class AddExprNode: public ExprNode{
    char type;
public:
    AddExprNode(char);
    virtual ~AddExprNode();
    virtual std::string translate(std::vector<std::string>&);
};

class MulExprNode: public ExprNode{
    char type;
public:
    MulExprNode(char);
    virtual ~MulExprNode();
    virtual std::string translate(std::vector<std::string>&);
};

class CallExprNode: public ExprNode{
public:
    CallExprNode(std::string);
    virtual ~CallExprNode();
    virtual std::string translate(std::vector<std::string>&);

    std::string name;
    std::vector<std::string> arg_list;
};

// variable references
class VarRef: public ExprNode{
public:
    VarRef(std::string, std::string);
    virtual ~VarRef();
    virtual std::string translate(std::vector<std::string>&);
    
    std::string type;
    std::string name;
};

class LitRef: public ExprNode{
public:
    LitRef(std::string, std::string);
    virtual ~LitRef();
    virtual std::string translate(std::vector<std::string>&);

    std::string type;
    std::string value;
};

class StmtNode{
public:
    StmtNode(){};
    virtual ~StmtNode(){};
    virtual std::vector<std::string>& translate()=0;
};

class AssignStmtNode: public StmtNode{
public:
    AssignStmtNode();
    virtual ~AssignStmtNode();
    std::vector<std::string>& translate();
    
    VarRef* to;
    ExprNode* from;
};

#endif
