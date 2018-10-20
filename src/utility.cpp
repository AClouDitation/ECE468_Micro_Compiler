#include "../src/utility.hpp"


// translate $T** to r**,
// probably remove in the future 
// and re-implement translate()
string t2r(string& t){
    return "r" + t.substr(2);
}

vector<string>& ir2tiny(vector<vector<string>>& irs){

    vector<string>* tiny = new vector<string>;

    for(auto items:irs){
        if(items[0] == "LABEL"){
            // TODO: fill this in the following steps;
        }
        else if(items[0] == "LINK"){
            // TODO: fill this in the following steps;
        }
        else if(items[0] == "ADDI"){
            string target_reg = t2r(items[3]); 
            string op1;
            string op2;

            if(items[1][0] == '$') op1 = t2r(items[1]);
            else op1 = items[1];

            if(items[2][0] == '$') op2 = t2r(items[2]);
            else op2 = items[2];

            string new_tiny_op = "move " + op1 + " " + target_reg;
            tiny->push_back(new_tiny_op);
            new_tiny_op = "addi " + op2 + " " + target_reg;
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "SUBI"){
            string target_reg = t2r(items[3]); 
            string op1;
            string op2;

            if(items[1][0] == '$') op1 = t2r(items[1]);
            else op1 = items[1];

            if(items[2][0] == '$') op2 = t2r(items[2]);
            else op2 = items[2];

            string new_tiny_op = "move " + op1 + " " + target_reg;
            tiny->push_back(new_tiny_op);
            new_tiny_op = "subi " + op2 + " " + target_reg;
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "MULI"){
            string target_reg = t2r(items[3]); 
            string op1;
            string op2;

            if(items[1][0] == '$') op1 = t2r(items[1]);
            else op1 = items[1];

            if(items[2][0] == '$') op2 = t2r(items[2]);
            else op2 = items[2];

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
            string op1;
            string op2;

            if(items[1][0] == '$') op1 = t2r(items[1]);
            else op1 = items[1];

            if(items[2][0] == '$') op2 = t2r(items[2]);
            else op2 = items[2];

            string new_tiny_op = "move " + op1 + " " + target_reg;
            tiny->push_back(new_tiny_op);
            new_tiny_op = "divi " + op2 + " " + target_reg;
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "ADDF"){
            string target_reg = t2r(items[3]); 
            string op1;
            string op2;

            if(items[1][0] == '$') op1 = t2r(items[1]);
            else op1 = items[1];

            if(items[2][0] == '$') op2 = t2r(items[2]);
            else op2 = items[2];

            string new_tiny_op = "move " + op1 + " " + target_reg;
            tiny->push_back(new_tiny_op);
            new_tiny_op = "addr " + op2 + " " + target_reg;
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "SUBF"){
            string target_reg = t2r(items[3]); 
            string op1;
            string op2;

            if(items[1][0] == '$') op1 = t2r(items[1]);
            else op1 = items[1];

            if(items[2][0] == '$') op2 = t2r(items[2]);
            else op2 = items[2];

            string new_tiny_op = "move " + op1 + " " + target_reg;
            tiny->push_back(new_tiny_op);
            new_tiny_op = "subr " + op2 + " " + target_reg;
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "MULF"){
            string target_reg = t2r(items[3]); 
            string op1;
            string op2;

            if(items[1][0] == '$') op1 = t2r(items[1]);
            else op1 = items[1];

            if(items[2][0] == '$') op2 = t2r(items[2]);
            else op2 = items[2];

            string new_tiny_op = "move " + op1 + " " + target_reg;
            tiny->push_back(new_tiny_op);
            new_tiny_op = "mulr " + op2 + " " + target_reg;
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "DIVF"){
            string target_reg = t2r(items[3]); 
            string op1;
            string op2;

            if(items[1][0] == '$') op1 = t2r(items[1]);
            else op1 = items[1];

            if(items[2][0] == '$') op2 = t2r(items[2]);
            else op2 = items[2];

            string new_tiny_op = "move " + op1 + " " + target_reg;
            tiny->push_back(new_tiny_op);
            new_tiny_op = "divr " + op2 + " " + target_reg;
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "STOREI"){
            string target_reg; 
            string op1;

            if(items[2][0] == '$') target_reg = t2r(items[2]);
            else target_reg = items[2];

            if(items[1][0] == '$') op1 = t2r(items[1]);
            else op1 = items[1];

            tiny->push_back("move " + op1 + " " + target_reg);
        }
        else if(items[0] == "STOREF"){
            string target_reg; 
            string op1;

            if(items[2][0] == '$') target_reg = t2r(items[2]);
            else target_reg = items[2];

            if(items[1][0] == '$') op1 = t2r(items[1]);
            else op1 = items[1];

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
            string op1;

            if(items[1][0] == '$') op1 = t2r(items[1]);
            else op1 = items[1];

            new_tiny_op += op1;
            tiny->push_back(new_tiny_op);
        }
        else if(items[0] == "WRITEF"){
            string new_tiny_op = "sys writer ";
            string op1;

            if(items[1][0] == '$') op1 = t2r(items[1]);
            else op1 = items[1];

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
