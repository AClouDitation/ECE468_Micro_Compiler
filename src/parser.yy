%{
    #include <cstdlib>
    #include <string>
    #include <stack>
    #include <vector>
    #include <iostream>
    #include <sstream> //for c++ 98 int to string conversion
    #include "../src/symtable.hpp"

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

}

%define api.token.prefix{TOK_}
%union {
    char*   cstr;
	int     ival;
	float   fval;
    std::string* sp;
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
%type <sp> id str var_type any_type 
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
                        $$ = new std::string($1);
                    };
pgm_body            :decl func_declarations;
decl                :string_decl decl|var_decl decl|/* empty */;

/* Global String Declaration */
string_decl         :STRING id ASSIGN str SEMICOLON {
                        Symtable* current = symtable_stack.top();
                        StrEntry* new_entry = new StrEntry(*$2,*$4);
                        current->add(new_entry);
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
                    };

id_tail             :COMMA id id_tail{
                        id_stack.push(*($2));
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
                    };
param_decl_tail     :COMMA param_decl param_decl_tail|/* empty */;

/* Function Declarations */
func_declarations   :func_decl func_declarations|/* empty */;
func_decl           :FUNCTION any_type id {
                        //std::cout << "fdecl:id:" << $3 << std::endl;
                        Symtable* current = new Symtable(*$3);
                        symtable_stack.push(current);
                        symtable_list.push_back(current);
                    }
                    OPAREN param_decl_list CPAREN BEGIN func_body END{
                        symtable_stack.pop();
                    };
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
if_stmt             :IF{
                        block_index++;
                        std::ostringstream temp;
                        temp << block_index;
                        std::string index_literal = temp.str();
                        //Symtable* current = new Symtable(
                        //    "BLOCK "+std::to_string(block_index));
                        Symtable* current = new Symtable(
                            "BLOCK "+ index_literal);
                        symtable_stack.push(current);
                        symtable_list.push_back(current);
                    } 
                    OPAREN cond CPAREN decl stmt_list {
                        symtable_stack.pop();
                    }
                    else_part ENDIF;
else_part           :ELSE{
                        block_index++;
                        std::ostringstream temp;
                        temp << block_index;
                        std::string index_literal = temp.str();
                        //Symtable* current = new Symtable(
                        //    "BLOCK "+std::to_string(block_index));
                        Symtable* current = new Symtable(
                            "BLOCK "+ index_literal);
                        symtable_stack.push(current);
                        symtable_list.push_back(current);
                    }
                    decl stmt_list{
                        symtable_stack.pop(); 
                    } 
                    | /* empty */;
cond                :expr compop expr | TRUE | FALSE;
compop              :LT | GT | EQ | NEQ | LEQ | GEQ;
while_stmt          :WHILE{
                        block_index++;
                        std::ostringstream temp;
                        temp << block_index;
                        std::string index_literal = temp.str();
                        //Symtable* current = new Symtable(
                        //    "BLOCK "+std::to_string(block_index));
                        Symtable* current = new Symtable(
                            "BLOCK "+ index_literal);
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
