#include "../inc/utility.hpp"
#include "../inc/ir/irNode.hpp"
#include "../inc/symtable/symtable.hpp"
#include<bits/stdc++.h>

using namespace std;

// split string 
// @s: string to split
// @c: delimiter
vector<string> SplitString(const string& s, const string& c){

    vector<string> v;
    string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(string::npos != pos2){
        v.push_back(s.substr(pos1, pos2-pos1));

        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length()) v.push_back(s.substr(pos1));

    return v;
}

// split ir string by space
// @irs: ir codes
vector<vector<string>>& split_irs(vector<string>& irs){
    vector<vector<string>>* sp_irs =
        new vector<vector<string>>;

    for(auto ir:irs){
        sp_irs->push_back(SplitString(ir," "));
    }

    return *sp_irs;
}

// translate !T** to r**,
// @t: temporary to translate
string t2r(string& t){
    if(t[0] == '!' && t[1] == 'T') return "r" + t.substr(2);    // if t is a temporary, return a register
    return t;                                                   // otherwise skip
}

// translate IR to tiny assembly
// @irs: vector<vector<string>> containing ir
vector<string>& ir2tiny(vector<vector<string>>& irs){

    vector<string>* tiny = new vector<string>;

    for(auto items:irs){
        if(items[0] == "LABEL"){
            tiny->push_back("label "+items[1]);
        }
        else if(items[0] == "LINK"){
            tiny->push_back("link "+items[1]);
        }
        else if(items[0] == "UNLINK"){
            tiny->push_back("unlnk");
        }
        else if(items[0] == "RET"){
            tiny->push_back("ret");
        }
        else if(items[0] == "JUMP"){
            tiny->push_back("jmp "+items[1]); 
        }
        else if(items[0] == "PUSH"){
            string pushOp = "push";
            if(items.size() == 2){
                pushOp += " " + t2r(items[1]);
            }
            tiny->push_back(pushOp);
        }
        else if(items[0] == "PUSHREGS") {
            // TODO: push all inuse registers on to the stack
        }
        else if(items[0] == "POP"){
            string popOp = "pop";
            if(items.size() == 2){
                popOp += " " + t2r(items[1]);
            }
            tiny->push_back(popOp);
        }
        else if(items[0] == "POPREGS") {
            // TODO: pop all used registers off the stack
        }
        else if(items[0] == "JSR"){
            tiny->push_back("jsr " + items[1]);
        }
        else if(items[0] == "MOVE"){
            string moveOp = "move ";
            string op1 = t2r(items[1]);
            string op2 = t2r(items[2]);

            tiny->push_back(moveOp + op1 + " " + op2);
        }
        else if(items[0] == "LTI" || items[0] == "LEI" ||
                items[0] == "GTI" || items[0] == "GEI" ||
                items[0] == "EQI" || items[0] == "NEI" ||
                items[0] == "LTF" || items[0] == "LEF" ||
                items[0] == "GTF" || items[0] == "GEF" ||
                items[0] == "EQF" || items[0] == "NEF")

        {
            string op1 = t2r(items[1]);
            string op2 = t2r(items[2]);

            string type = items[0][2] == 'I'?"i":"r";
            tiny->push_back("cmp" + type + " " + op1  + " " + op2);
            string cmp = "";
            cmp += items[0][0] + 'a' - 'A';
            cmp += items[0][1] + 'a' - 'A';
            tiny->push_back("j" + cmp + " " + items[3]);
        }
        else if(items[0] == "ADDI"){
            string target_reg = t2r(items[3]); 
            string op1 = t2r(items[1]);
            string op2 = t2r(items[2]);

            string new_tiny_op = "move " + op1 + " " + target_reg;
            tiny->push_back(new_tiny_op);
            new_tiny_op = "addi " + op2 + " " + target_reg;
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "SUBI"){
            string target_reg = t2r(items[3]); 
            string op1 = t2r(items[1]);
            string op2 = t2r(items[2]);

            string new_tiny_op = "move " + op1 + " " + target_reg;
            tiny->push_back(new_tiny_op);
            new_tiny_op = "subi " + op2 + " " + target_reg;
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "MULI"){
            string target_reg = t2r(items[3]); 
            string op1 = t2r(items[1]);
            string op2 = t2r(items[2]);

            string new_tiny_op;
            if(op1 == "1"){
                new_tiny_op = "move "+op2+" "+target_reg;     
                tiny->push_back(new_tiny_op);
            }
            else{
                new_tiny_op = "move " + op1 + " " + target_reg;
                tiny->push_back(new_tiny_op);
                if(op2 != "1"){
                    new_tiny_op = "muli " + op2 + " " + target_reg;
                    tiny->push_back(new_tiny_op);
                }
            }

        }
        else if(items[0] == "DIVI"){
            string target_reg = t2r(items[3]); 
            string op1 = t2r(items[1]);
            string op2 = t2r(items[2]);

            string new_tiny_op = "move " + op1 + " " + target_reg;
            tiny->push_back(new_tiny_op);
            new_tiny_op = "divi " + op2 + " " + target_reg;
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "ADDF"){
            string target_reg = t2r(items[3]); 
            string op1 = t2r(items[1]);
            string op2 = t2r(items[2]);

            string new_tiny_op = "move " + op1 + " " + target_reg;
            tiny->push_back(new_tiny_op);
            new_tiny_op = "addr " + op2 + " " + target_reg;
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "SUBF"){
            string target_reg = t2r(items[3]); 
            string op1 = t2r(items[1]);
            string op2 = t2r(items[2]);

            string new_tiny_op = "move " + op1 + " " + target_reg;
            tiny->push_back(new_tiny_op);
            new_tiny_op = "subr " + op2 + " " + target_reg;
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "MULF"){
            string target_reg = t2r(items[3]); 
            string op1 = t2r(items[1]);
            string op2 = t2r(items[2]);

            string new_tiny_op = "move " + op1 + " " + target_reg;
            tiny->push_back(new_tiny_op);
            new_tiny_op = "mulr " + op2 + " " + target_reg;
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "DIVF"){
            string target_reg = t2r(items[3]); 
            string op1 = t2r(items[1]);
            string op2 = t2r(items[2]);

            string new_tiny_op = "move " + op1 + " " + target_reg;
            tiny->push_back(new_tiny_op);
            new_tiny_op = "divr " + op2 + " " + target_reg;
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "STOREI"){
            string target_reg = t2r(items[2]); 
            string op1 = t2r(items[1]);

            tiny->push_back("move " + op1 + " " + target_reg);
        }
        else if(items[0] == "STOREF"){
            string target_reg = t2r(items[2]); 
            string op1 = t2r(items[1]);

            tiny->push_back("move " + op1 + " " + target_reg);
        }
        else if(items[0] == "READI"){
            string new_tiny_op = "sys readi ";
            new_tiny_op += items[1];
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "READF"){
            string new_tiny_op = "sys readr ";
            new_tiny_op += items[1];
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "WRITEI"){
            string new_tiny_op = "sys writei ";
            string op1 = t2r(items[1]);

            new_tiny_op += op1;
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "WRITEF"){
            string new_tiny_op = "sys writer ";
            string op1 = t2r(items[1]);

            new_tiny_op += op1;
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "WRITES"){
            string new_tiny_op = "sys writes ";
            new_tiny_op += items[1];
            tiny->push_back(new_tiny_op);
        }
    }

    return *tiny;
}

void irBlockInsert(vector<IrNode*>& block, IrNode* newIr) {
    IrNode* lastIr = block.empty()?NULL:block.back();
    block.push_back(newIr);
    if(lastIr && lastIr->cmd != "RET") {
        lastIr->setSuc(newIr); 
        newIr->setPre(lastIr);
    }
}

void irBlockCascade(vector<IrNode*>& block, vector<IrNode*>& newBlock) {
    IrNode* lastIr = block.empty()?NULL:block.back();
    IrNode* firstNewIr = newBlock.empty()?NULL:newBlock.front();
    if(lastIr && lastIr->cmd != "RET") {
        lastIr->setSuc(firstNewIr);
        if(firstNewIr) firstNewIr->setPre(lastIr);
    }
    block.insert(block.end(),newBlock.begin(),newBlock.end());
}

bool isLiteral(string op) {
    if(op[0] == '!' || op[0] == '$') return false;
    extern Symtable* globalSymtable;
    for (auto kv :globalSymtable->id_map) {
        if(kv.second->isFunc) continue;
        if(kv.second->type == "STRING") return true;
        if(kv.first == op) return false;
    }

    return true;
}

string toLower(string s){
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

bool is_constant(string lit){
    char have_dot = false;
    for(auto ch:lit){
        if(!isdigit(ch) && (ch != '.' || have_dot))return false;
        if(ch == '.') have_dot = true;
    }
    return true;
}
