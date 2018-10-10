#include <iostream>
#include "../src/opt.hpp"

std::vector<std::string> SplitString(const std::string& s, const std::string& c){

    std::vector<std::string> v;
    std::string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(std::string::npos != pos2){
        v.push_back(s.substr(pos1, pos2-pos1));

        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length()) v.push_back(s.substr(pos1));

    return v;
}

std::vector<std::vector<std::string>>& split_irs(std::vector<std::string>& irs){
    std::vector<std::vector<std::string>>* sp_irs = 
        new std::vector<std::vector<std::string>>;

    for(auto ir:irs){
        sp_irs->push_back(SplitString(ir," "));  
    }

    return *sp_irs;
}

void constant_swap(std::vector<std::string>& irs){
    std::vector<std::vector<std::string>> sp_irs = split_irs(irs);
    
}

void live_ana(std::vector<std::string>& irs){
    std::vector<std::string> live_list;
    for(auto ir:irs){
        std::cout << ir << std::endl;
    }

}
