#ifndef REGMAN_HPP_
#define REGMAN_HPP_

#include <unordered_map>
#include <vector>
#include <set>
#include <string>

class IrNode;
class regManager {
    int totalAmount;
    bool* isDirty;

    std::unordered_map<int, std::string> inUseOR;     
    std::unordered_map<std::string, int> inUseRO;     
public:
    regManager(int);
    virtual ~regManager();

    int regEnsure(std::string, std::vector<std::string>&, std::set<std::string>&);
    void regFree(int, std::vector<std::string>&, std::set<std::string>&);
    int regAllocate(std::string, std::vector<std::string>&, std::set<std::string>&);
    void markDirty(int);
    std::stringstream print();
};

#endif

