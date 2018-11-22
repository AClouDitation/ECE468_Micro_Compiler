#include "../../inc/regman.hpp"
#include "../../inc/irNode.hpp"
#include <iostream>

using namespace std;

regManager::regManager(int totalAmount):
    totalAmount(totalAmount) {

    isDirty = new bool[totalAmount];
    // push all avaliable register indexes into the priority queue
    // mark all register clean
    for(int i = 0; i < totalAmount;i++){
        ava.push(i);
        isDirty[i] = false;
    }
}

regManager::~regManager() {
    delete[] isDirty;
}

int regManager::regEnsure(string op) {
    if(inUseRO.find(op) != inUseRO.end()) return inUseRO[op];
    int reg = regAllocate(op);
    // FIXME: generate load from op to reg
    return reg;
}

void regManager::regFree(int r) {
    if(isDirty[r]){
    // FIXME: generate store
    }
    ava.push(r);
}

int regManager::regAllocate(string op) {
    
    // if there is a free register, return it
    if(!ava.empty()){
        int reg = ava.top();
        ava.pop();
        return reg;
    }
    
    // otherwise if there is a clean register, return it
    for(auto kv: inUseOR) {
        if(!isDirty[kv.first]) {
            regFree(kv.first);  // seems like dont actually need this
                                // since the register going to be used 
                                // after allocation anyway...
            return kv.first;
        }
    }

    // otherwise free r0
    regFree(0); 
    return 0;
}

void regManager::markDirty(int r){
    if(r >= totalAmount || r < 0) return;
    isDirty[r] = true;
}
