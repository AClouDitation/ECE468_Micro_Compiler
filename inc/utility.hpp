#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <string>
#include <string.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>

std::vector<std::string> SplitString(const std::string&, const std::string&);
std::vector<std::vector<std::string>>& split_irs(std::vector<std::string>& irs);
std::string t2r(std::string& t);
std::vector<std::string>& ir2tiny(std::vector<std::vector<std::string>>& irs);

class IrNode;
void irBlockInsert(std::vector<IrNode*>&, IrNode*);
void irBlockCascade(std::vector<IrNode*>&, std::vector<IrNode*>&);
bool isLiteral(std::string);
std::string toLower(std::string);

#endif
