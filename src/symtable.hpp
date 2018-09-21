#ifndef SYMTABLE_HPP
#define SYMTABLE_HPP
#include <vector>
#include <string>

class SymEntry{
    std::string name;
    std::string type;
    void* value;
public:
    SymEntry(std::string name, std::string type);
    SymEntry(std::string name, std::string type, void* value);
    virtual ~SymEntry();
    virtual void print();
};

class Symtable{

    std::vector<SymEntry> entrylist;
    std::string name;
public:
    Symtable(std::string name);
    virtual ~Symtable();
    virtual void add(SymEntry& SymEntry); 
    virtual void print();
};

#endif
