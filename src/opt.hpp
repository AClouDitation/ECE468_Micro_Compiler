#ifndef OPT_HPP
#define OPT_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include "../src/ast.hpp"

std::vector<std::string> SplitString(const std::string&, const std::string&);
std::vector<std::vector<std::string>>& split_irs(std::vector<std::string>& irs);

void OOOptmize(std::vector<std::vector<std::string>>&);
#endif
