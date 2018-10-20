%{
    #include <cstdlib>
    #include <string>
    #include <stack>
    #include <vector>
    #include <iostream>
    #include <bits/stdc++.h> //cstr
    #include "../src/symtable.hpp"
    #include "../src/ast.hpp"

    extern int yylex();
    extern int yylineno;
    extern char* yytext;
    void yyerror(const char* s);
%}

%code{
    int block_index = 0;
    std::stack<Symtable*> symtable_stack;
    std::vector<Symtable*> symtable_list;
    std::stack<std::string> id_stack;
    std::vector<StmtNode*> stmt_list;   //will remove
    std::vector<BlockNode*> block_list;


    // search through the symbol table,
    // return VarRef* with the name and type of the symbol.
    VarRef* find_id(std::string& id){
        std::stack<Symtable*> now_stack = symtable_stack;
        
        SymEntry* entry = NULL;

        while(!entry && !now_stack.empty()){
            Symtable* now = now_stack.top();
            entry = now -> have(id);    
            now_stack.pop();
        }
        
        if(!entry){
            //error handling
            yyerror("does not exist in scope!\n");
        }
        
        VarRef* new_ref = new VarRef(id, entry->type);
        return new_ref;
    }
}

%define api.token.prefix{TOK_}
%union {
    char*           cstr;
	int             ival;
	float           fval;
    char            ch;
    std::string*    sp;
    ExprNode*       en;
    StmtNode*       sn;
}

/* Keywords */
%token          PROGRAM
%token          BEGIN
%token          END
%token          FUNCTION
%token          READ
%token          WRITE
%token          IF
%token          ELSE
%token          ENDIF
%token          WHILE
%token          ENDWHILE
%token          RETURN
%token          INT
%token          VOID
%token          STRING
%token          FLOAT
%token          TRUE
%token          FALSE

/* Data type */
%token <cstr>    IDENTIFIER
%token <ival>    INTLITERAL
%token <fval>    FLOATLITERAL
%token <cstr>    STRINGLITERAL

%token          ASSIGN 
%token <cstr>   NEQ 
%token <cstr>   LEQ
%token <cstr>   GEQ 
%token <cstr>   PLUS
%token <cstr>   MINUS
%token <cstr>   MUL 
%token <cstr>   DIV
%token <cstr>   EQ
%token <cstr>   LT
%token <cstr>   GT 
%token          OPAREN
%token          CPAREN
%token          SEMICOLON
%token          COMMA

/*%type <entry> string_decl*/
%type <sp> id str var_type any_type 
%type <en> expr expr_prefix postfix_expr factor factor_prefix primary call_expr cond 
%type <sn> assign_expr 
%type <ch> addop mulop
%type <cstr> compop
%start program
%%
/* Grammar rules */
/* Program */
program             :PROGRAM {
                        Symtable* current = new Symtable("GLOBAL");
                        symtable_stack.push(current);
                        symtable_list.push_back(current);
                    }
                    id{delete $3;} BEGIN pgm_body END;

id                  :IDENTIFIER {
                        $$ = new std::string($1);
                    };
pgm_body            :decl func_declarations;
decl                :string_decl decl|var_decl decl|/* empty */;

/* Global String Declaration */
string_decl         :STRING id ASSIGN str SEMICOLON {
                        Symtable* current = symtable_stack.top();
                        StrEntry* new_entry = new StrEntry(*$2,*$4);
                        current->add(new_entry);
                        delete $2;
                        delete $4;
                    };
str                 :STRINGLITERAL{
                        $$ = new std::string($1);
                    };

/* Variable Declaration */
var_decl            :{
                        //init for reading variables
                        while(!id_stack.empty())id_stack.pop();
                    }
                    var_type id_list SEMICOLON {
                        Symtable* current = symtable_stack.top();
                        while(!id_stack.empty()){
                            if(*$2 == "INT"){
                                IntEntry* new_entry = new IntEntry(id_stack.top());
                                current->add(new_entry);
                            }
                            else{
                                FltEntry* new_entry = new FltEntry(id_stack.top());
                                current->add(new_entry);
                            }
                            id_stack.pop();
                        }
                        delete $2;
                    };
var_type            :FLOAT{
                        $$ = new std::string("FLOAT");  
                    }
                    |INT {
                        $$ = new std::string("INT");  
                    };
any_type            :var_type{
                        $$ = $1;
                    }    
                    |VOID{
                        $$ = new std::string("VOID");
                    }; 

id_list             :id id_tail {
                        id_stack.push(*($1));
                        delete $1;
                    };

id_tail             :COMMA id id_tail{
                        id_stack.push(*($2));
                        delete $2;
                    }
                    |/* empty */;

/* Function Paramater List */

param_decl_list     :param_decl param_decl_tail|/* empty */;
param_decl          :var_type id{
                        Symtable* current = symtable_stack.top(); 
                        if(*$1 == "INT"){
                            IntEntry* new_entry = new IntEntry(*$2);
                            current->add(new_entry);
                        }
                        else{
                            FltEntry* new_entry = new FltEntry(*$2);
                            current->add(new_entry);
                        }

                        delete $1;
                        delete $2;
                    };
param_decl_tail     :COMMA param_decl param_decl_tail|/* empty */;

/* Function Declarations */
func_declarations   :func_decl func_declarations|/* empty */;
func_decl           :FUNCTION any_type id {
                        Symtable* current = new Symtable(*$3);
                        symtable_stack.push(current);
                        symtable_list.push_back(current);
                        FunctionDeclNode* new_func = new FunctionDeclNode(*$3,*$2,current);
                        block_list.push_back(new_func);
                        // for now
                        delete $2;
                        delete $3;
                    }
                    OPAREN param_decl_list CPAREN BEGIN func_body END{
                        symtable_stack.pop();
                        block_list.pop_back();
                    };
func_body           :decl stmt_list;

/* Statement List */
stmt_list           :stmt stmt_list|/* empty */;
stmt                :base_stmt|if_stmt|loop_stmt; 
base_stmt           :assign_stmt|read_stmt|write_stmt|control_stmt;

/* Basic Statements */
assign_stmt         :assign_expr SEMICOLON{
                        block_list.back()->stmt_list.push_back($1); 
                    };

assign_expr         :id ASSIGN expr{
                        AssignStmtNode* new_assign = new AssignStmtNode(); 

                        // search the current symbol stack to find the table;
                        VarRef* to = find_id(*$1);
                        delete $1;  //free memory of id

                        // $3 should return a ExprNode*
                        new_assign -> to = to;
                        new_assign -> from = $3;
                        $$ = new_assign;
                    };
read_stmt           :{
                        while(!id_stack.empty())id_stack.pop();
                    }
                    READ OPAREN id_list CPAREN SEMICOLON{
                        ReadStmtNode* new_read = new ReadStmtNode();
                        while(!id_stack.empty()){
                            new_read->id_list.push_back(find_id(id_stack.top()));
                            id_stack.pop();
                        }
                        block_list.back()->stmt_list.push_back(new_read);
                    };
write_stmt          :{
                        while(!id_stack.empty())id_stack.pop();
                    }WRITE OPAREN id_list CPAREN SEMICOLON{
                        WriteStmtNode* new_write = new WriteStmtNode();
                        while(!id_stack.empty()){
                            new_write->id_list.push_back(find_id(id_stack.top()));
                            id_stack.pop();
                        }
                        block_list.back()->stmt_list.push_back(new_write);

                        //do something
                    };
return_stmt         :RETURN expr SEMICOLON;

/* Expressions */
expr                :expr_prefix factor {
                        if($1){
                            $1 -> rnode = $2; // add right oprand to the exprnode
                            $$ = $1;
                        }
                        else $$ = $2;
                    };
expr_prefix         :expr_prefix factor addop {
                        $$ = new AddExprNode($3);
                        if($1){
                            $1 -> rnode = $2;
                            $$ -> lnode = $1;
                        }
                        else $$ -> lnode = $2; 
                    } | /* empty */{$$ = NULL;};
factor              :factor_prefix postfix_expr {
                        if($1){
                            $$ = $1;
                            $$ -> rnode = $2;
                        }
                        else $$ = $2;
                    };
factor_prefix       :factor_prefix postfix_expr mulop {
                        $$ = new MulExprNode($3);
                        if($1){
                            $$ -> lnode = $1;
                            $1 -> rnode = $2;
                        }
                        else $$ -> lnode = $2; 
                    } | /* empty */{$$ = NULL;};
postfix_expr        :primary {
                        $$ = $1;
                    } | call_expr {
                        $$ = $1;
                    };
call_expr           :id OPAREN expr_list CPAREN {
                        // TODO: make a function call!
                        // dont care for now...
                        CallExprNode* new_call = new CallExprNode(*$1);
                        delete $1;
                        $$ = new_call;
                    };
expr_list           :expr expr_list_tail | /* empty */;
expr_list_tail      :COMMA expr expr_list_tail | /* empty */;
primary             :OPAREN expr CPAREN {
                        $$ = $2; 
                    } | id{
                        VarRef* new_var = find_id(*$1);
                        delete $1;
                        $$ = new_var;
                    } | INTLITERAL {
                        LitRef* new_lit = new LitRef("INT",
                            std::to_string(static_cast<long long int>($1)));
                        $$ = new_lit;
                    } | FLOATLITERAL {
                        LitRef* new_lit = new LitRef("FLOAT",
                            std::to_string(static_cast<long double>($1)));
                        $$ = new_lit;
                    };
addop               :PLUS{$$='+';} | MINUS{$$='-';};
mulop               :MUL{$$='*';} | DIV{$$='/';};

/* Complex Statements and Condition */ 
if_stmt             :IF OPAREN cond CPAREN decl{
                        // allocate a new block
                        block_index++;
                        Symtable* current = new Symtable(
                            "BLOCK "+
                            std::to_string(static_cast<long long int>(block_index)));
                        symtable_stack.push(current);
                        symtable_list.push_back(current);

                        // allocate a new if node
                        IfStmtNode* new_if = new IfStmtNode(dynamic_cast<CondExprNode*>($3),current);
                        block_list.back()->stmt_list.push_back(new_if); 
                        block_list.push_back(new_if);
                    }
                    stmt_list{
                        symtable_stack.pop();                    
                        block_list.pop_back();
                    }
                    else_part ENDIF;
else_part           :ELSE{
                        block_index++;
                        Symtable* current = new Symtable(
                            "BLOCK "+
                            std::to_string(static_cast<long long int>(block_index)));
                        symtable_stack.push(current);
                        symtable_list.push_back(current);
                    }
                    decl stmt_list{
                        symtable_stack.pop(); 
                    } 
                    | /* empty */;
cond                :expr compop expr{
                        CondExprNode* new_cond = new CondExprNode((string)$2);
                        new_cond->lnode = $1;
                        new_cond->rnode = $3;
                        $$ = new_cond;
                    }
                    | TRUE {
                        CondExprNode* new_lit = new CondExprNode("TRUE");
                    }
                    | FALSE{
                        CondExprNode* new_lit = new CondExprNode("FALSE");
                    };
compop              :LT | GT | EQ | NEQ | LEQ | GEQ; /* reutrn $1 by default */
while_stmt          :WHILE{
                        block_index++;
                        Symtable* current = new Symtable(
                            "BLOCK "+
                            std::to_string(static_cast<long long int>(block_index)));
                        symtable_stack.push(current);
                        symtable_list.push_back(current);
                    } 
                    OPAREN cond CPAREN decl stmt_list ENDWHILE{
                        symtable_stack.pop(); 
                    };


/*ECE468 ONLY*/
control_stmt        :return_stmt;
loop_stmt           :while_stmt;

%%
//Epilouge
void yyerror (const char* s){
    //fprintf(stderr,"Error Line %d token %s\n",yylineno,s);
    std::cout << "Not Accepted" << std::endl;
    exit(1);
}
