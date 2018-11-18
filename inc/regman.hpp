#ifndef REGMAN_HPP_
#define REGMAN_HPP_

#include <queue>
#include <unordered_map>
#include <vector>
#include <string>

class regManager {
    int totalAmount;
    std::priority_queue<int,std::vector<int>, std::greater<int>> ava;
    std::unordered_map<int, std::string> inUse;     
public:
    regManager(int);
    virtual ~regManager();
    std::string takeReg(std::string var);
    std::vector<std::string> inUseList();
};

#endif

