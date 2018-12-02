#ifndef REGMAN_HPP_
#define REGMAN_HPP_

#include <unordered_map>
#include <vector>
#include <set>
#include <string>
#include <sstream>

class IrNode;
class FunctionDeclNode;

class regManager {
    int totalAmount;
    bool* isDirty;
    FunctionDeclNode* farther;

    std::unordered_map<int, std::string> inUseOR;     
    std::unordered_map<std::string, int> inUseRO;     
    std::unordered_map<std::string, int> tempLoc;     

    regManager* bk;
public:
    regManager(const regManager&);
    regManager(int, FunctionDeclNode*);
    virtual ~regManager();

    int regEnsure(std::string, int, std::vector<std::string>&, std::set<std::string>&);
    void regEnsure2(std::string, std::string, int&, int&, 
            std::vector<std::string>&, std::set<std::string>&);
    void regFree(int, std::vector<std::string>&, std::set<std::string>&);
    int regAllocate(std::string, int, std::vector<std::string>&, std::set<std::string>&);

    void markDirty(int);
    void freeGlobal(std::vector<std::string>&);
    void freeReturn(std::vector<std::string>&, int);

    void pushAll();
    void popAll();

    std::stringstream print();
};

#endif

