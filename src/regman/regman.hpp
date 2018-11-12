#ifndef REGMAN_HPP_
#define REGMAN_HPP_

#include <queue>
#include <set>
#include <vector>
#include <string>

class regManager {
    int totalAmount;
    std::priority_queue<int,std::vector<int>, std::greater<int>> ava;
    std::set<int> inUse;
public:
    regManager(int);
    virtual ~regManager();
    std::string takeReg();
    std::vector<std::string> inUseList();
};

#endif

