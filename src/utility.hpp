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
std::string t2r(std::string& t);
std::vector<std::string>& ir2tiny(std::vector<std::vector<std::string>>& irs);

#endif
