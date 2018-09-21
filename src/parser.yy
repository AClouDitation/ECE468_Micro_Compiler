%{
    #include <cstdlib>
    #include <string>
    #include <stack>
    #include <vector>
    #include <iostream>
    #include "../src/symtable.hpp"

    extern int yylex();
    extern int yylineno;
    extern char* yytext;
    std::string typenow;
    void yyerror(const char* s);
%}

%code{
    int block_index = 0;
    std::stack<Symtable*> symtable_stack;
    std::vector<Symtable*> symtable_list;
}

%define api.token.prefix{TOK_}
%union {
    char*   cstr;
	int     ival;
	float   fval;
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
%token <cstr>   INT
%token <cstr>   VOID
%token <cstr>   STRING
%token <cstr>   FLOAT
%token          TRUE
%token          FALSE

/* Data type */
%token <cstr>    IDENTIFIER
%token <ival>    INTLITERAL
%token <fval>    FLOATLITERAL
%token <cstr>    STRINGLITERAL

%token          ASSIGN 
%token          NEQ 
%token          LEQ
%token          GEQ 
%token          PLUS
%token          MINUS 
%token          MUL 
%token          DIV
%token          EQ
%token          LT
%token          GT 
%token          OPAREN
%token          CPAREN
%token          SEMICOLON
%token          COMMA

/*%type <entry> string_decl*/
%type <cstr> id str var_type
%start program
%%
/* Grammar rules */
/* Program */
program             :PROGRAM {
                        Symtable* current = new Symtable("GLOBAL");
                        symtable_stack.push(current);
                        symtable_list.push_back(current);
                    }
                    id BEGIN pgm_body END;

id                  :IDENTIFIER{
                        std::cout << "id :" << $1 << std::endl;
                        $$ = $1;
                    };
pgm_body            :decl func_declarations;
decl                :string_decl decl|var_decl decl|/* empty */{
                        std::cout << "BAKA!" << std::endl;
                    };

/* Global String Declaration */
string_decl         :STRING id ASSIGN str SEMICOLON {
                        std::string id($2);
                        SymEntry new_entry(id, "STRING",(void*)$4);
                        Symtable* current = symtable_stack.top();
                        current->add(new_entry);
                    };
str                 :STRINGLITERAL{
                        $$ = $1;
                    };

/* Variable Declaration */
var_decl            :var_type{
                        typenow = $1;
                    }
                    id_list SEMICOLON{
                        
                        //Symtable* current = symtable_list.top();
                        //SymEntry new_entry(id, type);
                        //current->add(new_entry);
                    };
var_type            :FLOAT|INT{$$=$1;};
any_type            :var_type|VOID; 
id_list             :id {
                        std::cout << "add ids" << std::endl;
                        Symtable* current = symtable_stack.top();
                        SymEntry new_entry($1, typenow);
                        current->add(new_entry);
                    }
                    id_tail;
id_tail             :COMMA id{
                        Symtable* current = symtable_stack.top();
                        SymEntry new_entry($2, typenow);
                        current->add(new_entry);
                    }id_tail|/* empty */ ;

/* Function Paramater List */

param_decl_list     :param_decl param_decl_tail|/* empty */;
param_decl          : var_type id;
param_decl_tail     :COMMA param_decl param_decl_tail|/* empty */;

/* Function Declarations */
func_declarations   :func_decl func_declarations|/* empty */;
func_decl           :FUNCTION any_type id {
                        std::cout << "fdecl:id:" << $3 << std::endl;
                        Symtable* current = new Symtable(std::string($3));
                        symtable_stack.push(current);
                        symtable_list.push_back(current);
                    }
                    OPAREN param_decl_list CPAREN BEGIN func_body END{
                        symtable_stack.pop();
                    };
func_body           :decl {std::cout << "end of decl" << std::endl;} stmt_list{std::cout << "end of stmt_list" << std::endl;};

/* Statement List */
stmt_list           :stmt stmt_list|/* empty */;
stmt                :base_stmt|if_stmt|loop_stmt; 
base_stmt           :assign_stmt|read_stmt|write_stmt|control_stmt;

/* Basic Statements */
assign_stmt         :assign_expr SEMICOLON;
assign_expr         :id ASSIGN expr;
read_stmt           :READ OPAREN id_list CPAREN SEMICOLON;
write_stmt          :WRITE OPAREN id_list CPAREN SEMICOLON;
return_stmt         :RETURN expr SEMICOLON;

/* Expressions */
expr                :expr_prefix factor;
expr_prefix         :expr_prefix factor addop | /* empty */;
factor              :factor_prefix postfix_expr;
factor_prefix       :factor_prefix postfix_expr mulop | /* empty */;
postfix_expr        :primary | call_expr;
call_expr           :id OPAREN expr_list CPAREN;
expr_list           :expr expr_list_tail | /* empty */;
expr_list_tail      :COMMA expr expr_list_tail | /* empty */;
primary             :OPAREN expr CPAREN | id | INTLITERAL | FLOATLITERAL;
addop               :PLUS | MINUS;
mulop               :MUL | DIV;

/* Complex Statements and Condition */ 
if_stmt             :IF OPAREN cond CPAREN decl stmt_list else_part ENDIF;
else_part           :ELSE decl stmt_list | /* empty */;
cond                :expr compop expr | TRUE | FALSE;
compop              :LT | GT | EQ | NEQ | LEQ | GEQ;
while_stmt          :WHILE OPAREN cond CPAREN decl stmt_list ENDWHILE;


/*ECE468 ONLY*/
control_stmt        :return_stmt;
loop_stmt           :while_stmt;

%%
//Epilouge
void yyerror (const char* s){
    //fprintf(stderr,"Error Line %d token %s\n",yylineno,s);
    fprintf(stdout,"Not Accepted");
    exit(1);
}
