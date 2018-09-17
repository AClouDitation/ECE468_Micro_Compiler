#include "symtable.hpp"
#include <iostream>

SymEntry::SymEntry(std::string& name, std::string& type){
    this->name = name;
    this->type = type;
    this->value = NULL;
}

SymEntry::SymEntry(std::string& name, std::string& type, void* value){
    this->name = name;
    this->type = type;
    this->value = value;
}

SymEntry::~SymEntry(){}

void SymEntry::print(){
    std::cout<<name<<" "<<type;
    if(value){
        std::cout<<(char*)value<<std::endl;
    }
    else{
        std::cout<<std::endl;
    }
}

Symtable::Symtable(std::string name){this->name = name;}
Symtable::~Symtable(){}
void Symtable::add(SymEntry& entry){
    entrylist.push_back(entry);
}

void Symtable::print(){
    for(int i = 0;i < entrylist.size();i++)
        entrylist[i].print();
}
