#ifndef OPT_HPP
#define OPT_HPP

#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

vector<string> SplitString(const string&, const string&);
vector<vector<string>>& split_irs(vector<string>& irs);

void OOOptmize(vector<vector<string>>&);
#endif
