#include "symtable.hpp"
#include <iostream>
#include <cstdlib>

using namespace std;

SymEntry::SymEntry(string name, string type){
    this->name = name;
    this->type = type;
}

SymEntry::~SymEntry(){}

void SymEntry::print(){
    cout<<"name "<<name<<" type "<<type<<endl;
}

// different entry types
StrEntry::StrEntry(string name, string lit)
    :SymEntry(name,"STRING")
{
    literal = lit;
}
void StrEntry::print(){
    cout<<"name "<<name<<" type STRING"<<" value "<<literal<<endl;
}

IntEntry::IntEntry(string name):SymEntry(name,"INT"){}

FltEntry::FltEntry(string name):SymEntry(name,"FLOAT"){}

FuncEntry::FuncEntry(string name, string type, int argc):
    SymEntry(name, type),argc(argc){}

void FuncEntry::print(){
    cout<<"Function name "<<name<<" type "<< type << endl;
}

int FuncEntry::getArgCnt(){
    return argc;
}
// the symtable table
Symtable::Symtable(string name):name(name){}
Symtable::~Symtable(){
    for(auto id:id_map) delete id.second;
}

void Symtable::add(SymEntry* entry){
    
    if(id_map.find(entry->name)==id_map.end()){
        id_map[entry->name] = entry;
    }
    else{
        cout<<"DECLARATION ERROR "<<entry->name<<endl;
        exit(1);
    }
}

void Symtable::print(){
    cout << "Symbol table " << name << endl;
    for(auto id:id_map)    id.second->print();
}

vector<string>& Symtable::decl(){
    vector<string>* decl_ops = new vector<string>;
    for(auto kv:id_map){
        string new_decl = "";
        if(kv.second->type == "STRING"){
            new_decl += "str " + kv.first + " "
                + dynamic_cast<StrEntry*>(kv.second)->literal;
        }
        else{
            new_decl += "var "+kv.first;
        }
        decl_ops->push_back(new_decl);
    }

    return *decl_ops;
}

SymEntry* Symtable::have(string id){
    if(id_map.find(id) != id_map.end()) return id_map[id];
    return NULL;
}

