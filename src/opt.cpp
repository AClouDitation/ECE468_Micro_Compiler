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

// cross_out dead exprs
void _cross_out(unordered_map<string,string>& reg_content, string& target) {
    //cerr << "Looking for " << target << endl;
    for(auto it = reg_content.begin();it != reg_content.end();){
        vector<string>items = SplitString(it->first," ");
        //cerr << items[0] << " " << items[1] << " " << items[2] << endl;
        if(target == items[1] || target == items[2]){
            //cerr << "RM " << it->first << endl;
            it = reg_content.erase(it);        
        }
        else it++;
    }
}

// perform backward liveness analysis
void live_ana(vector<vector<string>>& irs) {    

    unordered_map<string,string> reg_content;
    //                  ^expr       ^reg

    for(auto ir = irs.begin(); ir != irs.end();) {

        if((*ir)[0] == "READI" || (*ir)[0] == "READF")
        {
            string target = (*ir)[1];
            _cross_out(reg_content, target);
        }
        else if((*ir)[0] == "STOREI" || (*ir)[0] == "STOREF")
        {
            string target = (*ir)[2];
            _cross_out(reg_content, target);
        }
        else if((*ir)[0] == "MULI" || (*ir)[0] == "MULF" ||
                (*ir)[0] == "ADDI" || (*ir)[0] == "ADDF" ||
                (*ir)[0] == "DIVI" || (*ir)[0] == "DIVF" ||
                (*ir)[0] == "SUBI" || (*ir)[0] == "SUBF")
        {
            string target = (*ir)[3];
            string content = (*ir)[0] + " " + (*ir)[1] + " " + (*ir)[2];
            if(reg_content.find(content) == reg_content.end())
            {
                reg_content[content] = target;
            }
            else
            {
                string target = (*ir)[3];
                (*ir).pop_back();
                (*ir)[0] = "STORE" + (*ir)[0].substr(3);
                (*ir)[1] = reg_content[content];
                (*ir)[2] = target;
            }
            _cross_out(reg_content, target);
        }
        ir++;
    }
}


void dead_store_eli(vector<vector<string>>& irs){

    for(auto ir = irs.begin(); ir != irs.end();ir++){
        if((*ir)[0] == "STOREI" || (*ir)[0] == "STOREF"){
            // go through the code to see what happens next...
         
            bool need_this_store = false;
            string target = (*ir)[2];
            auto nir = ir;
            nir++;
            for(;nir != irs.end();nir++){

                if((*nir)[0] == "READI" || (*nir)[0] == "READF")
                {
                    string new_target = (*nir)[1];
                    if(new_target == target)break;
                }
                else if((*nir)[0] == "STOREI" || (*nir)[0] == "STOREF")
                {
                    string new_target = (*nir)[2];
                    string op1 = (*nir)[1];
                    if(op1 == target){need_this_store = true;break;}
                    if(new_target == target)break;
                }
                else if((*nir)[0] == "MULI" || (*nir)[0] == "MULF" ||
                        (*nir)[0] == "ADDI" || (*nir)[0] == "ADDF" ||
                        (*nir)[0] == "DIVI" || (*nir)[0] == "DIVF" ||
                        (*nir)[0] == "SUBI" || (*nir)[0] == "SUBF")
                {
                    string op1 = (*nir)[1];
                    string op2 = (*nir)[2];
                    string new_target = (*ir)[3];
                    if(op1 == target || op2 == target){need_this_store = true;break;}
                    if(new_target == target)break;
                }
                else if((*nir)[0] == "WRITEI" || (*nir)[0] == "WRITEF"){
                    string new_target = (*nir)[1];
                    if(new_target == target){need_this_store = true;break;}
                }
            }
             
            if(!need_this_store){
                ir = irs.erase(ir);
                ir--;
            }
        }
    }
}

void OOOptmize(vector<vector<string>>& irs){
    constant_swap(irs);
    live_ana(irs);

    unsigned int len;
    do{
        len = irs.size();
        dead_store_eli(irs);
    }while(irs.size() < len);
}
