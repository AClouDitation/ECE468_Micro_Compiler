#include "../inc/utility.hpp"
#include "../inc/opt.hpp"

using namespace std;


bool _is_constant(string lit){
    char have_dot = false;
    for(auto ch:lit){
        if(!isdigit(ch) && (ch != '.' || have_dot))return false;
        if(ch == '.') have_dot = true;
    }
    return true;
}

void constant_swap(vector<vector<string>>& irs)
{
    unordered_map<string, string> const_refs;

    for(auto ir = irs.begin(); ir != irs.end();)
    {
        if((*ir)[0] == "STOREI" || (*ir)[0] == "STOREF")
        {
            if(_is_constant((*ir)[1]))// op1 is a literal
            {
                const_refs[(*ir)[2]] = (*ir)[1];
                ir = irs.erase(ir);
            }
            else if(const_refs.find((*ir)[1]) != const_refs.end()) // or const ref
            {
                const_refs[(*ir)[2]] = const_refs[(*ir)[1]];
                ir = irs.erase(ir);
            }
            else
            {
                const_refs.erase((*ir)[1]);    
                ir++;
            }
        }
        else if((*ir)[0] == "READI" || (*ir)[0] == "READF"){
            const_refs.erase((*ir)[1]);    // not a constant anymore
            ir++;
        }
        else if((*ir)[0] == "MULI" || (*ir)[0] == "MULF" ||
                (*ir)[0] == "ADDI" || (*ir)[0] == "ADDF" ||
                (*ir)[0] == "DIVI" || (*ir)[0] == "DIVF" ||
                (*ir)[0] == "SUBI" || (*ir)[0] == "SUBF")
        {
            if((_is_constant((*ir)[1]) ||   // op1 is a literal
                        const_refs.find((*ir)[1]) != const_refs.end()) && // or const ref
                    (_is_constant((*ir)[2]) || 
                     const_refs.find((*ir)[2]) != const_refs.end()))
            {
                // op1 and op2 are constant
                if((*ir)[0][3] == 'I'){
                    int op1 = _is_constant((*ir)[1])?stoi((*ir)[1]):stoi(const_refs[(*ir)[1]]);
                    int op2 = _is_constant((*ir)[2])?stoi((*ir)[2]):stoi(const_refs[(*ir)[2]]);
                    int res;
                    if((*ir)[0] == "MULI") res = op1*op2;
                    if((*ir)[0] == "ADDI") res = op1+op2;
                    if((*ir)[0] == "DIVI") res = op1/op2;
                    if((*ir)[0] == "SUBI") res = op1-op2;

                    const_refs[(*ir)[3]] = to_string(res);
                    ir = irs.erase(ir);
                }
                else{
                    double op1 = _is_constant((*ir)[1])?stof((*ir)[1]):stof(const_refs[(*ir)[1]]);
                    double op2 = _is_constant((*ir)[2])?stof((*ir)[2]):stof(const_refs[(*ir)[2]]);
                    double res;
                    if((*ir)[0] == "MULF") res = op1*op2;
                    if((*ir)[0] == "ADDF") res = op1+op2;
                    if((*ir)[0] == "DIVF") res = op1/op2;
                    if((*ir)[0] == "SUBF") res = op1-op2;

                    const_refs[(*ir)[3]] = to_string(res);
                    ir = irs.erase(ir);

                }
            }
            else{
                if(const_refs.find((*ir)[1]) != const_refs.end())
                    (*ir)[1] = const_refs[(*ir)[1]];
                if(const_refs.find((*ir)[2]) != const_refs.end())
                    (*ir)[2] = const_refs[(*ir)[2]];
                const_refs.erase((*ir)[3]);
                ir++;
            }
        }
        else if((*ir)[0] == "WRITEI" || (*ir)[0] == "WRITEF"){
            if(const_refs.find((*ir)[1]) != const_refs.end()){
                //store back to register for priting
                vector<string> str_ir;
                if((*ir)[0] == "WRITEI")str_ir.push_back("STOREI");
                else if((*ir)[0] == "WRITEF")str_ir.push_back("STOREF");
                str_ir.push_back(const_refs[(*ir)[1]]);
                //TODO: fix this with register manager
                //str_ir.push_back("$T"+to_string(temp_reg_index)); // store to register
                //(*ir)[1] = "$T" + to_string(temp_reg_index++);
                irs.insert(ir,str_ir);
                ir++;
            }
            ir++;
        }
        else{
            // should be WRITES only
            ir++;
        }


    }
}

void OOOptmize(vector<vector<string>>& irs){
    constant_swap(irs);

}
