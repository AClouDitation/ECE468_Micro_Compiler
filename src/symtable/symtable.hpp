#ifndef SYMTABLE_HPP
#define SYMTABLE_HPP
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <iostream>

class SymEntry{
public:
    std::string name;
    std::string type;
    SymEntry(std::string, std::string);
    virtual ~SymEntry();
    virtual void print();
};

class StrEntry: public SymEntry{
public:
    std::string literal;
    StrEntry(std::string, std::string);
    virtual ~StrEntry(){};
    void print();
};

class IntEntry: public SymEntry{
public:
    IntEntry(std::string);
    virtual ~IntEntry(){};
};

class FltEntry: public SymEntry{
public:
    FltEntry(std::string);
    virtual ~FltEntry(){};
};

class FuncEntry: public SymEntry{
    int argc;
public:
    FuncEntry(std::string, std::string, int);
    virtual ~FuncEntry(){};
    void print();

    virtual int getArgCnt();
};

class Symtable{
    std::unordered_map<std::string, SymEntry*> id_map; // this is better
    std::string name;
public:
    Symtable(std::string name);
    virtual ~Symtable();
    virtual void add(SymEntry*); 
    virtual void print();
    virtual std::vector<std::string>& decl();
    virtual SymEntry* have(std::string);
};

#endif
