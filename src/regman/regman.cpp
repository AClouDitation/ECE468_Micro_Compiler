#include "../../inc/regman/regman.hpp"
#include "../../inc/symtable/symtable.hpp"
#include <iostream>
#include <iomanip>

using namespace std;

regManager::regManager(const regManager& orig) {
    totalAmount = orig.totalAmount;
    isDirty = new bool[totalAmount];
    for(int i = 0;i < totalAmount;i++) {
        isDirty[i] = orig.isDirty[i];
    }

    farther = orig.farther;
    inUseOR = orig.inUseOR;
    inUseRO = orig.inUseRO;
    tempLoc = orig.tempLoc;

    bk = NULL;
}

regManager::regManager(int totalAmount, FunctionDeclNode* farther):
    totalAmount(totalAmount), farther(farther) {

    isDirty = new bool[totalAmount];
    // mark all register clean
    for(int i = 0; i < totalAmount;i++) isDirty[i] = false;
}

regManager::~regManager() {
    delete[] isDirty;
}

int regManager::regEnsure(string op, int doNotFree, vector<string>& opcode, set<string>& liveOut) {
    if(inUseRO.find(op) != inUseRO.end()) return inUseRO[op];
    int reg = regAllocate(op, doNotFree, opcode, liveOut);
    // generate load from op to reg
    if(tempLoc.find(op) != tempLoc.end()) op = "$-" + to_string(tempLoc[op]);
    opcode.push_back("move " + op + " r" + to_string(reg));
    isDirty[reg] = false;
    return reg;
}


void regManager::regEnsure2(string op1, string op2, int& reg1, int& reg2, 
        vector<string>& opcode, set<string>& liveOut) {
    
    reg1 = -1;
    reg2 = -1;

    if(inUseRO.find(op1) != inUseRO.end()) reg1 = inUseRO[op1];
    if(inUseRO.find(op2) != inUseRO.end()) reg2 = inUseRO[op2];

    if(reg1 == -1) reg1 = regEnsure(op1, reg2, opcode, liveOut);
    if(reg2 == -1) reg2 = regEnsure(op2, reg1, opcode, liveOut);
}

void regManager::regFree(int r, vector<string>& opcode, set<string>& liveOut) {
    // if not used, return directly
    if(inUseOR.find(r) == inUseOR.end()) return;

    //if r dirty and var in r still live, generate store to spill the register
    if(isDirty[r] && liveOut.find(inUseOR[r]) != liveOut.end()) {
        // map extra temporary onto stack
        if(inUseOR[r][0] == '!' && tempLoc.find(inUseOR[r]) == tempLoc.end())
            tempLoc[inUseOR[r]] = ++(farther->stackSize);
        
        string loc = inUseOR[r];
        if(tempLoc.find(loc) != tempLoc.end()) loc = "$-" + to_string(tempLoc[loc]);
        opcode.push_back("move r" + to_string(r) + " " + loc);
    }

    // mark r as free
    isDirty[r] = false;
    inUseRO.erase(inUseRO.find(inUseOR[r]));
    inUseOR.erase(inUseOR.find(r));
}

int regManager::regAllocate(string op, int doNotFree, 
        vector<string>& opcode, set<string>& liveOut) {
    
    int reg = -1;

    // if there is a free register, choose it
    for(int i = 0;i < totalAmount;i++) {
        if(inUseOR.find(i) == inUseOR.end() && i != doNotFree) {
            reg = i;
            break;
        }
    }
    
    // otherwise if there is a clean register, choose it
    if(reg == -1) {
        for(auto kv: inUseOR) {
            if(isDirty[kv.first]) continue;
            if(kv.first == doNotFree) continue;
            regFree(kv.first, opcode, liveOut);      
            reg = kv.first;
            break;
        }
    }

    // otherwise choose r0
    if(reg == -1) {
        if(doNotFree != 0) {
            reg = 0;
            regFree(0, opcode, liveOut); 
        }
        else {
            reg = 1;
            regFree(1, opcode, liveOut); 
        }
    }

    // mark r associated with op
    if(inUseOR.find(reg) != inUseOR.end() && inUseRO.find(inUseOR[reg]) != inUseRO.end())
        inUseRO.erase(inUseRO.find(inUseOR[reg]));
    inUseOR[reg] = op;
    inUseRO[op] = reg;
    return reg;
}

void regManager::markDirty(int r) {
    if(r >= totalAmount || r < 0) return;
    isDirty[r] = true;
}

void regManager::freeGlobal(vector<string>& opCodeBlock) {

    // if there is global variable still in use, spill it
    extern Symtable* globalSymtable;
    for(auto kv: globalSymtable->id_map) {
        if(kv.second->isFunc) continue;
        
        if(inUseRO.find(kv.first) != inUseRO.end())
            opCodeBlock.push_back("move r" + to_string(inUseRO[kv.first]) + " " + kv.first);
    }
}

void regManager::freeReturn(vector<string>& opCodeBlock, int retLoc) {
    string op = "$" + to_string(retLoc);
    // if return value still in use, spill it
    if(inUseRO.find(op) != inUseRO.end())
        opCodeBlock.push_back("move r" + to_string(inUseRO[op]) + " " + op);
}

stringstream regManager::print() {
   stringstream ss;

   for(int i = 0;i < 4; i++){
       ss << "r" + to_string(i) + ": ";
       ss << left << setfill(' ') << setw(10);
       if(inUseOR.find(i) != inUseOR.end()) ss << inUseOR[i] + (isDirty[i]?"*":"");
       else ss << (isDirty[i]?"*":"");
   }

   return ss;
}

void regManager::pushAll() {
    bk = new regManager(*this);
}

void regManager::popAll() {
    totalAmount = bk->totalAmount;
    for(int i = 0; i < totalAmount;i++) {
        isDirty[i] = bk->isDirty[i];
    }
    farther = bk->farther;
    inUseOR = bk->inUseOR;
    inUseRO = bk->inUseRO;
    tempLoc = bk->tempLoc;

    delete bk;
    bk = NULL;
}
