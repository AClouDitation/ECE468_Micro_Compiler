%{
    #include <cstdlib>
    #include <string>
    #include <stack>
    #include <vector>
    #include <iostream>
    #include <bits/stdc++.h> //cstr
    #include "../inc/symtable.hpp"
    #include "../inc/ExprNode.hpp"
    #include "../inc/StmtNode.hpp"
    #include "../inc/BaseStmtNode.hpp"

    extern int yylex();
    extern int yylineno;
    extern char* yytext;
    void yyerror(const char* s);
%}

%code{
    int block_index = 0;
    int if_else_label_index = 0;
    Symtable* globalSymtable;
    std::stack<Symtable*> symtable_stack;
    std::stack<std::string> id_stack;
    std::vector<StmtNode*> stmt_list;   //will remove
    std::vector<BlockNode*> block_list;
    std::vector<FunctionDeclNode*> func_list;
    std::stack<ExprNode*>* expr_stack_ptr; // used when calling function...
    std::stack<std::stack<ExprNode*>*> expr_stack_ptr_stack; // this is soooo f**king dumb
                                                             // however has to be here for the f**king sake
                                                             // of nested function call


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
            std::cerr << id;
            yyerror(" does not exist in scope!\n");
        }
        
        VarRef* new_ref;
        if(now_stack.empty()){  // reaching the last Symbol table, var is global
            new_ref = new VarRef(func_list.back(), id, entry->type);
        }
        else{                   // var is local other wise, referencing by adding offset on framepointer
            new_ref = new VarRef(func_list.back(), "$"+std::to_string(entry->index), entry->type);
        }
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
%type <ival> param_decl_list param_decl_tail
%start program
%%
/* Grammar rules */
/* Program */
program             :PROGRAM {
                        Symtable* current = new Symtable("GLOBAL");
                        globalSymtable = current;
                        symtable_stack.push(current);
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

param_decl_list     :param_decl param_decl_tail{$$ = $2 + 1;}
                    |/* empty */{$$ = 0;};

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
param_decl_tail     :COMMA param_decl param_decl_tail{$$ = $3 + 1;}|/* empty */{$$ = 0;};

/* Function Declarations */
func_declarations   :func_decl func_declarations|/* empty */;
func_decl           :FUNCTION any_type id {
                        // add function declaration to the symbol table
                        Symtable* current = symtable_stack.top();
                        FuncEntry* new_entry = new FuncEntry(*$3, *$2); 
                        current->add(new_entry); 
                        // allocate symboltable for the new function
                        symtable_stack.push(new Symtable(*$3));
                    }OPAREN param_decl_list CPAREN {
                        
                        // set argc for the new function
                        FuncEntry* thisFunc = static_cast<FuncEntry*>(globalSymtable->have(*$3));
                        assert(thisFunc);
                        thisFunc->setArgCnt($6);

                        // get the symbol table of this function
                        Symtable* current = symtable_stack.top();
                        current->offsetFuncParam();
                        FunctionDeclNode* new_func = new FunctionDeclNode(*$3,*$2,$6,current);

                        block_list.push_back(new_func);
                        func_list.push_back(new_func); // for easier to find them in main

                        // for now
                        delete $2;
                        delete $3;
                    }
                    BEGIN func_body END{
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
                        AssignStmtNode* new_assign = new AssignStmtNode(func_list.back()); 

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
                        ReadStmtNode* new_read = new ReadStmtNode(func_list.back());
                        while(!id_stack.empty()){
                            new_read->id_list.push_back(find_id(id_stack.top()));
                            id_stack.pop();
                        }
                        block_list.back()->stmt_list.push_back(new_read);
                    };
write_stmt          :{
                        while(!id_stack.empty())id_stack.pop();
                    }WRITE OPAREN id_list CPAREN SEMICOLON{
                        WriteStmtNode* new_write = new WriteStmtNode(func_list.back());
                        while(!id_stack.empty()){
                            new_write->id_list.push_back(find_id(id_stack.top()));
                            id_stack.pop();
                        }
                        block_list.back()->stmt_list.push_back(new_write);

                        //do something
                    };
return_stmt         :RETURN expr SEMICOLON{
                        // interesting
                        // seems like expr is guaranteed not empty
                        
                        FunctionDeclNode* lastFuncDecl = func_list.back();
                        assert(lastFuncDecl);
                        int retLoc = lastFuncDecl->argc + 2 + 4;    // only have 4 registers
                        
                        block_list.back()->stmt_list.push_back(new ReturnStmtNode(lastFuncDecl, $2, retLoc)); 
                    };

/* Expressions */
expr                :expr_prefix factor {
                        if($1){
                            $1 -> rnode = $2; // add right oprand to the exprnode
                            $$ = $1;
                        }
                        else $$ = $2;
                    };
expr_prefix         :expr_prefix factor addop {
                        $$ = new AddExprNode(func_list.back(),$3);
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
                        $$ = new MulExprNode(func_list.back(),$3);
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
call_expr           :id{
                        // TODO:check if the amount of argument match 
                        if(!globalSymtable->have(*$1)){
                            std::cout << "undeclared function " << *$1 << std::endl;
                            yyerror("undeclared function"); 
                        }
                        // push last expr list onto stack
                        if(expr_stack_ptr) expr_stack_ptr_stack.push(expr_stack_ptr);
                        // allocate new expr list
                        expr_stack_ptr = new std::stack<ExprNode*>();
                    }OPAREN expr_list CPAREN {
                        
                        CallExprNode* new_call = new CallExprNode(func_list.back(), *$1);
                        new_call->exprStack = *expr_stack_ptr;  // copy expr list in to call expr node

                        // free and pop
                        delete expr_stack_ptr;
                        if(!expr_stack_ptr_stack.empty()) {
                            expr_stack_ptr = expr_stack_ptr_stack.top();
                            expr_stack_ptr_stack.pop();
                        }

                        delete $1;
                        $$ = new_call;
                    };
expr_list           :expr expr_list_tail {
                        expr_stack_ptr->push($1);  
                    }| /* empty */;
expr_list_tail      :COMMA expr expr_list_tail {
                        expr_stack_ptr->push($2);  
                    }| /* empty */;
primary             :OPAREN expr CPAREN {
                        $$ = $2; 
                    } | id{
                        VarRef* new_var = find_id(*$1);
                        delete $1;
                        $$ = new_var;
                    } | INTLITERAL {
                        LitRef* new_lit = new LitRef(func_list.back(),"INT",std::to_string($1));
                        $$ = new_lit;
                    } | FLOATLITERAL {
                        LitRef* new_lit = new LitRef(func_list.back(),"FLOAT",std::to_string($1));
                        $$ = new_lit;
                    };
addop               :PLUS{$$='+';} | MINUS{$$='-';};
mulop               :MUL{$$='*';} | DIV{$$='/';};

/* Complex Statements and Condition */ 
if_stmt             :IF OPAREN cond CPAREN decl{
                        // allocate a new block
                        block_index++;
                        Symtable* current = new Symtable("BLOCK "+std::to_string(block_index));
                        symtable_stack.push(current);

                        // allocate a new if node
                        IfStmtNode* new_if = new IfStmtNode(dynamic_cast<CondExprNode*>($3),current,
                            std::to_string(if_else_label_index++), func_list.back());
                        block_list.back()->stmt_list.push_back(new_if); 
                        block_list.push_back(new_if);
                    }
                    stmt_list{
                        symtable_stack.pop();                    
                    }
                    else_part ENDIF{
                        block_list.pop_back();
                    };
else_part           :ELSE{
                        block_index++;
                        Symtable* current = new Symtable(
                            "BLOCK "+
                            std::to_string(block_index));
                        symtable_stack.push(current);
                        //symtable_list.push_back(current);

                        // allocate a new else node
                        ElseStmtNode* new_else = new ElseStmtNode(current, func_list.back());
                        dynamic_cast<IfStmtNode*>(block_list.back())->elseNode = new_else;
                        block_list.push_back(new_else);
                    }
                    decl stmt_list{
                        symtable_stack.pop(); 
                        block_list.pop_back();
                    } 
                    | /* empty */;
cond                :expr compop expr{
                        CondExprNode* new_cond = new CondExprNode(func_list.back(), (std::string)$2);
                        new_cond->lnode = $1;
                        new_cond->rnode = $3;
                        $$ = new_cond;
                    }
                    | TRUE {
                        CondExprNode* new_lit = new CondExprNode(func_list.back(), "TRUE");
                    }
                    | FALSE{
                        CondExprNode* new_lit = new CondExprNode(func_list.back(), "FALSE");
                    };
compop              :LT| GT| EQ| NEQ| LEQ| GEQ; /* reutrn $1 by default */
while_stmt          :WHILE OPAREN cond CPAREN {
                        block_index++;
                        Symtable* current = new Symtable(
                            "BLOCK "+
                            std::to_string(block_index));
                        symtable_stack.push(current);

                        // allocate a new while node
                        WhileStmtNode* new_while = new WhileStmtNode(dynamic_cast<CondExprNode*>($3),current,
                            std::to_string(static_cast<long long int>(block_index)), func_list.back());
                        block_list.back()->stmt_list.push_back(new_while); 
                        block_list.push_back(new_while);
                    } 
                    decl stmt_list ENDWHILE{
                        symtable_stack.pop(); 
                        block_list.pop_back();
                    };


/*ECE468 ONLY*/
control_stmt        :return_stmt;
loop_stmt           :while_stmt;

%%
//Epilouge
void yyerror (const char* s){   
    std::cout << s << std::endl;
    std::cout << "Not Accepted" << std::endl;
    exit(1);
}
