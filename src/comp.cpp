#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stack>
#include "../src/symtable.hpp"
#include "../src/ast.hpp"
#include "../generated/parser.hpp"


extern FILE* yyin;
extern std::stack<Symtable*> symtable_stack; // should be empty,
                                             // contents should be freed when they
                                             // were poped
extern std::vector<Symtable*> symtable_list; // this is redundant 
                                             // however needed in step3
extern std::vector<StmtNode*> stmt_list;    


void update_AST_type(ExprNode* root){
    if(!root->lnode && !root->rnode) return; // this is a Ref Node
    if(root->lnode) update_AST_type(root->lnode);
    if(root->rnode) update_AST_type(root->rnode);

    if(root->lnode->type == "FLOAT" || root->rnode->type == "FLOAT"){
        root->type = "FLOAT"; 
    }
}


std::vector<std::string>& AST2IR(){
    std::vector<std::string>* ir = new std::vector<std::string>;
    for(auto stmt: stmt_list){
        update_AST_type(dynamic_cast<AssignStmtNode*>(stmt)->from);
        std::vector<std::string> code_block = stmt->translate();
        ir->insert(ir->end(),code_block.begin(),code_block.end());

       
    }

    return *ir;
}
int main(int argc, char** argv){
    
    FILE* fp = fopen(argv[1],"r");
    yyin = fp;
    yyparse();

    /* for step 3
    for(int i = 0;i < symtable_list.size();i++){
        symtable_list[i]->print();
        if(i != symtable_list.size()-1)std::cout << std::endl;
    }        
    */

    for(auto table:symtable_list){
        delete table;
    }

    std::vector<std::string>& ir = AST2IR(); 

    // for debugging
    for(auto line: ir){
        std::cout << line << std::endl;
    }
    //


    fclose(fp);

    return 0;
}
