#include "../../inc/livenessAnalysis.hpp"

using namespace std;

vector<set<string>> liveAna(vector<vector<string>>& irs) {    

    vector<set<string>> liveVarsList;
    set<string> liveVars;
    for(auto it = irs.rend(); it != irs.rbegin(); it++){
        string gen;
        vector<string> use;
        vector<string> items = *it;
        if(items[0] == "LABEL"){

        }
        else if(items[0] == "LINK"){
        }
        else if(items[0] == "UNLINK"){
        }
        else if(items[0] == "RET"){
        }
        else if(items[0] == "JUMP"){
        }
        else if(items[0] == "PUSH"){
            if(items.size() == 2){
                string op1 = items[1];
                liveVars.insert(op1);
            }
        }
        else if(items[0] == "PUSHREGS") {
            // TODO: push all inuse registers on to the stack
        }
        else if(items[0] == "POP"){
            if(items.size() == 2){
                string op1 = items[1];
                liveVars.erase(op1);
            }
        }
        else if(items[0] == "POPREGS") {
            // TODO: pop all used registers off the stack
        }
        else if(items[0] == "JSR"){
        }
        else if(items[0] == "MOVE"){
            string op1 = items[1];
            string op2 = items[2];
            liveVars.insert(op2);
            liveVars.insert(op1);
        }
        else if(items[0] == "LTI" || items[0] == "LEI" ||
                items[0] == "GTI" || items[0] == "GEI" ||
                items[0] == "EQI" || items[0] == "NEI" ||
                items[0] == "LTF" || items[0] == "LEF" ||
                items[0] == "GTF" || items[0] == "GEF" ||
                items[0] == "EQF" || items[0] == "NEF")
        {
            string op1 = items[1];
            string op2 = items[2];
            liveVars.insert(op1);
            liveVars.insert(op2);
        }
        else if(items[0] == "ADDI" || items[0] == "ADDF" ||
                items[0] == "SUBI" || items[0] == "SUBF" ||
                items[0] == "MULI" || items[0] == "MULF" ||
                items[0] == "DIVI" || items[0] == "DIVF")
        {
            string op1 = items[1];
            string op2 = items[2];
            string op3 = items[3];
            liveVars.erase(op3);
            liveVars.insert(op1);
            liveVars.insert(op2);
        }
        else if(items[0] == "STOREI" || items[0] == "STROEF"){
            string op1 = items[1];
            string op2 = items[1];
            liveVars.erase(op2);
            liveVars.insert(op1);
        }
        else if(items[0] == "READI" || items[0] == "READF"){
            string op1 = items[1];
        }
        else if(items[0] == "WRITEI" || items[0] == "WRITEF"){
            string op1 = items[1];
            liveVars.insert(op1);
        }
        else if(items[0] == "WRITES"){
        }
        liveVarsList.insert(liveVarsList.begin(),liveVars);
    }

    return liveVarsList;
}


