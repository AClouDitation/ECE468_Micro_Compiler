#include "regman.hpp"
#include <iostream>

using namespace std;
regManager::regManager(int totalAmount):
    totalAmount(totalAmount) {
    for(int i = 0; i < totalAmount;i++){
        ava.push(i);
    }
}

regManager::~regManager() {}

string regManager::takeReg() {
    int ret = ava.top();    // get the next available register
    ava.pop();              // remove it from available registers
    inUse.insert(ret);      // add it to the inUse register set
    return "$T"+to_string(ret);
}

vector<string> regManager::inUseList() {
    vector<string> ret;
    for(auto reg: inUse) {
        ret.push_back("$T"+to_string(reg));
    }
    return ret;
}



