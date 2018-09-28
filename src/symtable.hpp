#ifndef SYMTABLE_HPP
#define SYMTABLE_HPP
#include <vector>
#include <unordered_set>
#include <string>
#include <iostream>

class SymEntry{
public:
    std::string name;
    SymEntry(std::string name);
    virtual ~SymEntry();
    virtual void print()=0;
};

class StrEntry: public SymEntry{
    std::string literal;
public:
    StrEntry(std::string name, std::string lit);
    virtual ~StrEntry(){};
    void print();
};

class IntEntry: public SymEntry{
public:
    IntEntry(std::string name);
    virtual ~IntEntry(){};
    void print();
};

class FltEntry: public SymEntry{
public:
    FltEntry(std::string name);
    virtual ~FltEntry(){};
    void print();
};


class Symtable{

    std::vector<SymEntry*> entrylist;
    std::unordered_set<std::string> id_set;
    std::string name;
public:
    Symtable(std::string name);
    virtual ~Symtable();
    virtual void add(SymEntry* SymEntry); 
    virtual void print();
};

#endif
