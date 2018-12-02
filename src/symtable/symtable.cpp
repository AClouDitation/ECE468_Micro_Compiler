#include "../../inc/symtable/symtable.hpp"
#include <iostream>
#include <cstdlib>

using namespace std;

SymEntry::SymEntry(string name, string type):
    isFunc(false), name(name), type(type){}

SymEntry::~SymEntry(){}

void SymEntry::print(){
    cout<<"name "<<name<<" type "<<type<<endl;
}

// different entry types
StrEntry::StrEntry(string name, string lit)
    :SymEntry(name,"S")
{
    literal = lit;
}
void StrEntry::print(){
    cout<<"name "<<name<<" type STRING"<<" value "<<literal<<endl;
}

IntEntry::IntEntry(string name):SymEntry(name,"I"){}

FltEntry::FltEntry(string name):SymEntry(name,"R"){}

FuncEntry::FuncEntry(string name, string type):
    SymEntry(name, type),argc(0){isFunc=true;}

void FuncEntry::print(){
    cout<<"Function name "<<name<<" type "<< type << endl;
}

void FuncEntry::setArgCnt(int argc){this->argc = argc;};
int FuncEntry::getArgCnt(){return argc;}

// the symtable table
// start from 2 since $1 is the frame pointer 
Symtable::Symtable(string name):name(name),nextIndex(-2){}

Symtable::~Symtable(){
    for(auto id:id_map) delete id.second;
}

void Symtable::add(SymEntry* entry){
    
    if(id_map.find(entry->name)==id_map.end()){
        entry->index = nextIndex--;
        id_map[entry->name] = entry;
    }
    else{
        cout<<"DECLARATION ERROR "<<entry->name<<endl;
        exit(1);
    }
}

void Symtable::print(){
    cout << "Symbol table " << name << endl;
    for(auto id:id_map) id.second->print();
}

vector<string>& Symtable::decl(){
    vector<string>* decl_ops = new vector<string>;
    for(auto kv:id_map){
        string new_decl = "";
        if(kv.second->isFunc) continue;
        if(kv.second->type == "S"){
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

int Symtable::size() {return id_map.size();}

void Symtable::offsetFuncParam(){
   for(auto entry: id_map) {
        // entry.second->index *= -1;
        entry.second->index -= (nextIndex-1);
   }
   nextIndex = -1;
}

