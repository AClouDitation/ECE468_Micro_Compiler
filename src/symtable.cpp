#include "symtable.hpp"
#include <iostream>
#include <cstdlib>

SymEntry::SymEntry(std::string name, std::string type){
    this->name = name;
    this->type = type;
}
SymEntry::~SymEntry(){}


// different entry types
StrEntry::StrEntry(std::string name, std::string lit)
    :SymEntry(name,"STRING")
{
    literal = lit;
}
void StrEntry::print(){
    std::cout<<"name "<<name<<" type STRING"<<" value "<<literal<<std::endl;
}

IntEntry::IntEntry(std::string name):SymEntry(name,"INT"){}
void IntEntry::print(){
    std::cout<<"name "<<name<<" type INT"<<std::endl;
}

FltEntry::FltEntry(std::string name):SymEntry(name,"FLOAT"){}
void FltEntry::print(){
    std::cout<<"name "<<name<<" type FLOAT"<<std::endl;
}


// the symtable table
Symtable::Symtable(std::string name){this->name = name;}
Symtable::~Symtable(){
    for(unsigned int i=0;i < entrylist.size(); i++){
        delete entrylist[i];
    }
}

void Symtable::add(SymEntry* entry){
    
    entrylist.push_back(entry); // will remove
    /*
    if(id_set.find(entry->name)==id_set.end()){
        id_set.insert(entry->name);
    }
    else{
        std::cout<<"DECLARATION ERROR "<<entry->name<<std::endl;
        std::exit(1);
    }
    */
    if(id_map.find(entry->name)==id_map.end()){
        id_map[entry->name] = entry;
    }
    else{
        std::cout<<"DECLARATION ERROR "<<entry->name<<std::endl;
        std::exit(1);
    }
}

void Symtable::print(){
    std::cout << "Symbol table " << name << std::endl;
    for(unsigned int i = 0;i < entrylist.size();i++)
        entrylist[i]->print();
}

SymEntry* Symtable::have(std::string id){
    if(id_map.find(id) != id_map.end()) return id_map[id];
    return NULL;
}

