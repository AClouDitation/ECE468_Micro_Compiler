#include "regman.hpp"
#include <iostream>

using namespace std;

regManager::regManager(int totalAmount):
    totalAmount(totalAmount) {

    // push all avaliable register indexes into the 
    // priority queue
    for(int i = 0; i < totalAmount;i++){
        ava.push(i);
    }
}

regManager::~regManager() {}

// request a register for a variable/stack reference
// @var: variable name/stack location <$#>
string regManager::takeReg(string var) {
    int nextAva;
    if(!ava.empty()) {
        nextAva = ava.top();                // get the next available register
        ava.pop();                          // remove it from available registers
    }
    else{
        nextAva = 0;
        // TODO: spill one register
    }
    inUse.insert(make_pair(nextAva,var));   // add it to the inUse register set
    return "!T"+to_string(nextAva);
}

// return a vector containing all register in use
vector<string> regManager::inUseList() {
    vector<string> ret;
    for(auto kv: inUse) {
        int reg = kv.first;
        ret.push_back("!T"+to_string(reg));
    }
    return ret;
}



