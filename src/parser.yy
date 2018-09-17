%{
    #include <cstdlib>
    #include <string>
    #include <stack>
    #include "../src/symtable.hpp"

    extern int yylex();
    extern int yylineno;
    extern char* yytext;
    void yyerror(const char* s);
%}

%code{
    int block_index = 0;
    std::stack<Symtable*> symtable_list;
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
%token          INT
%token          VOID
%token <cstr>   STRING
%token          FLOAT
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
%type <cstr> id str
%start program
%%
/* Grammar rules */
/* Program */
program             :PROGRAM id BEGIN pgm_body END{
                        Symtable* current = new Symtable("Global");
                        symtable_list.push(current);
                    };
id                  :IDENTIFIER{};
pgm_body            :decl func_declarations;
decl                :string_decl decl|var_decl decl|/* empty */;

/* Global String Declaration */
string_decl         :STRING id ASSIGN str SEMICOLON {
                        std::string id($2);
                        std::string type($1);
                        SymEntry new_entry(id, type,(void*)$4);
                        Symtable* current = symtable_list.top();
                        current->add(new_entry);
                    };
str                 :STRINGLITERAL{};

/* Variable Declaration */
var_decl            :var_type id_list SEMICOLON;
var_type            :FLOAT|INT;
any_type            :var_type|VOID; 
id_list             :id id_tail;
id_tail             :COMMA id id_tail|/* empty */ ;

/* Function Paramater List */
param_decl_list     :param_decl param_decl_tail|/* empty */;
param_decl          : var_type id;
param_decl_tail     :COMMA param_decl param_decl_tail|/* empty */;

/* Function Declarations */
func_declarations   :func_decl func_declarations|/* empty */;
func_decl           :FUNCTION any_type id OPAREN param_decl_list CPAREN BEGIN func_body END;
func_body           :decl stmt_list;

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
