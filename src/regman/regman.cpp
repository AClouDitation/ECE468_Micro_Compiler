#include "../../inc/regman.hpp"
#include "../../inc/irNode.hpp"
#include <iostream>
#include <iomanip>

using namespace std;

regManager::regManager(int totalAmount):
    totalAmount(totalAmount) {

    isDirty = new bool[totalAmount];
    // mark all register clean
    for(int i = 0; i < totalAmount;i++) isDirty[i] = false;
}

regManager::~regManager() {
    delete[] isDirty;
}

int regManager::regEnsure(string op, vector<string>& opcode, set<string>& liveOut) {
    if(inUseRO.find(op) != inUseRO.end()) return inUseRO[op];
    int reg = regAllocate(op, opcode, liveOut);
    // generate load from op to reg
    opcode.push_back("move " + op + " r" + to_string(reg));
    return reg;
}

void regManager::regFree(int r, vector<string>& opcode, set<string>& liveOut) {
    // if not used, return directly
    if(inUseOR.find(r) == inUseOR.end()) return;

    //if r dirty and var in r still live, generate store to spill the register
    if(isDirty[r] && liveOut.find(inUseOR[r]) != liveOut.end())
        opcode.push_back("move r" + to_string(r) + " " + inUseOR[r]);

    // mark r as free
    inUseRO.erase(inUseRO.find(inUseOR[r]));
    inUseOR.erase(inUseOR.find(r));
}

int regManager::regAllocate(string op, vector<string>& opcode, set<string>& liveOut) {
    
    int reg = -1;

    // if there is a free register, choose it
    for(int i = 0;i < totalAmount;i++) {
        if(inUseOR.find(i) == inUseOR.end()) {
            reg = i;
            break;
        }
    }
    
    // otherwise if there is a clean register, choose it
    if(reg == -1) {
        for(auto kv: inUseOR) {
            if(isDirty[kv.first]) continue;
            regFree(kv.first, opcode, liveOut);      
            reg = kv.first;
            break;
        }
    }

    // otherwise choose r0
    if(reg == -1) {
        reg = 0;
        regFree(0, opcode, liveOut); 
    }

    // mark r associated with op
    if(inUseOR.find(reg) != inUseOR.end() && inUseRO.find(inUseOR[reg]) != inUseRO.end())
        inUseRO.erase(inUseRO.find(inUseOR[reg]));
    inUseOR[reg] = op;
    inUseRO[op] = reg;
    return reg;
}

void regManager::markDirty(int r){
    if(r >= totalAmount || r < 0) return;
    isDirty[r] = true;
}

stringstream regManager::print() {
   stringstream ss;

   for(int i = 0;i < 4; i++){
       ss << "r" + to_string(i) + ": ";
       ss << left << setfill(' ') << setw(10);
       if(inUseOR.find(i) != inUseOR.end()) ss << inUseOR[i];
       else ss << ' ';
   }

   return ss;
}
