#ifndef FUNCTIONDECLNODE_HPP_
#define FUNCTIONDECLNODE_HPP_

#include "BlockNode.hpp"
class FunctionDeclNode: public BlockNode{
    int nextAvaTemp;
public:
    std::string name;
    std::string type;
    
    int argc;
    int stackSize;

    regManager* regMan;
    FunctionDeclNode(std::string, std::string, int, Symtable* symtable);
    virtual ~FunctionDeclNode();

    virtual std::vector<IrNode*>& translate();

    std::string getNextAvaTemp();
    int getStackSize();
};

#endif
