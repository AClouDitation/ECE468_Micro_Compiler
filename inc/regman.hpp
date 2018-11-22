#ifndef REGMAN_HPP_
#define REGMAN_HPP_

#include <queue>
#include <unordered_map>
#include <vector>
#include <string>

class IrNode;
class regManager {
    int totalAmount;
    bool* isDirty;
    std::priority_queue<int, std::vector<int>, std::greater<int>> ava;
    std::unordered_map<int, std::string> inUseOR;     
    std::unordered_map<std::string, int> inUseRO;     
public:
    regManager(int);
    virtual ~regManager();

    int regEnsure(std::string);
    void regFree(int);
    int regAllocate(std::string);
    void markDirty(int);
};

#endif

