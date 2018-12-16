# ECE468 project Micro Compiler
This course focuses on the tools and techniques needed to build an optimizing compiler. 
Topics include:

1. Scanning and parsing: determining the syntactic structure of a program
2. Semantic routines: determining the semantics of a program and building an intermediate representation
3. Code generation: emitting assembly code that is equivalent to the program
4. Program optimizations: improving the performance of a program
5. Program analysis: determining interesting information about a program’s behavior

The following standards comes from the course website, which you can find here:
https://engineering.purdue.edu/EE468/project/

## Token definitions
```
an IDENTIFIER token will begin with a letter, and be followed by any number of letters and numbers.
IDENTIFIERS are case sensitive.

INTLITERAL: integer number
            ex) 0, 123, 678
FLOATLITERAL: floating point number available in two different format
                yyyy.xxxxxx or .xxxxxxx
            ex) 3.141592 , .1414 , .0001 , 456.98

STRINGLITERAL: any sequence of characters except '"'
            between '"' and '"'
            ex) "Hello world!" , "***********" , "this is a string"

COMMENT:
      Starts with "--" and lasts till the end of line
      ex) -- this is a comment
      ex) -- any thing after the "--" is ignored


Keywords

PROGRAM,BEGIN,END,FUNCTION,READ,WRITE,
IF,ELSE,ENDIF,WHILE,ENDWHILE,RETURN,INT,
VOID,STRING,FLOAT,TRUE,FALSE

573 Students add the following keywords:
FOR,ENDFOR,CONTINUE,BREAK

Operators
:= + - * / = != < > ( ) ; , <= >=
```

## Context-free grammars

```
/* Program */
program           -> PROGRAM id BEGIN pgm_body END
id                -> IDENTIFIER
pgm_body          -> decl func_declarations
decl		        -> string_decl decl | var_decl decl | empty

/* Global String Declaration */
string_decl       -> STRING id := str ;
str               -> STRINGLITERAL

/* Variable Declaration */
var_decl          -> var_type id_list ;
var_type	        -> FLOAT | INT
any_type          -> var_type | VOID
id_list           -> id id_tail
id_tail           -> , id id_tail | empty

/* Function Paramater List */
param_decl_list   -> param_decl param_decl_tail | empty
param_decl        -> var_type id
param_decl_tail   -> , param_decl param_decl_tail | empty

/* Function Declarations */
func_declarations -> func_decl func_declarations | empty
func_decl         -> FUNCTION any_type id (param_decl_list) BEGIN func_body END
func_body         -> decl stmt_list

/* Statement List */
stmt_list         -> stmt stmt_list | empty
stmt              -> base_stmt | if_stmt | loop_stmt
base_stmt         -> assign_stmt | read_stmt | write_stmt | control_stmt

/* Basic Statements */
assign_stmt       -> assign_expr ;
assign_expr       -> id := expr
read_stmt         -> READ ( id_list );
write_stmt        -> WRITE ( id_list );
return_stmt       -> RETURN expr ;

/* Expressions */
expr              -> expr_prefix factor
expr_prefix       -> expr_prefix factor addop | empty
factor            -> factor_prefix postfix_expr
factor_prefix     -> factor_prefix postfix_expr mulop | empty
postfix_expr      -> primary | call_expr
call_expr         -> id ( expr_list )
expr_list         -> expr expr_list_tail | empty
expr_list_tail    -> , expr expr_list_tail | empty
primary           -> ( expr ) | id | INTLITERAL | FLOATLITERAL
addop             -> + | -
mulop             -> * | /

/* Complex Statements and Condition */
if_stmt           -> IF ( cond ) decl stmt_list else_part ENDIF
else_part         -> ELSE decl stmt_list | empty
cond              -> expr compop expr | TRUE | FALSE
compop            -> < | > | = | != | <= | >=
while_stmt        -> WHILE ( cond ) decl stmt_list ENDWHILE


/*ECE468 ONLY*/
control_stmt      -> return_stmt
loop_stmt         -> while_stmt

/*ECE573 ONLY*/
control_stmt      -> return_stmt | CONTINUE ; | BREAK ;
loop_stmt         -> while_stmt | for_stmt
init_stmt         -> assign_expr | empty
incr_stmt         -> assign_expr | empty
for_stmt          -> FOR ( init_stmt ; cond ; incr_stmt ) decl stmt_list ENDFOR
```

## BUILD
Simply make
```make
make compiler_local -j4
```
you can change -j4 to whatever # of threads you would like to use

## HOW TO COMPILE MICRO CODE
```bash
./compiler <your_micro_code> > <tiny_assembly>
```

## HOW TO RUN
```bash
./tiny4regs <tiny_assembly>
```
or if you want to use file as input, simply do
```bash
cat <your_input> | ./tiny4regs <tiny_assemby>
```

## EXAMPLE CODE
Here is a minimal example
```
PROGRAM expr
BEGIN

	INT a,b,c,d,e;
	STRING newline := "\n";

	FUNCTION VOID main()
	BEGIN
		a := 1;
		b := 2;

          READ(c, d);

          e := a * c + b * d;
          
          WRITE(c, newline, d, newline, e, newline);

	END	
END
```

And a more entertaining one
```
PROGRAM factorial2
BEGIN
	STRING input := "Please enter an integer number: ";
	STRING eol := "\n";
	
	FUNCTION INT factorial(INT a)
	BEGIN
		INT ret;
		INT tret;
		IF (a = 1)
			RETURN 1;
		ELSE
			tret := factorial(a-1);
			ret := tret*a;
		ENDIF
		RETURN ret;
	END

	FUNCTION INT main ()
	BEGIN
		INT num;
		INT result;
		WRITE(input);
		READ(num);
		
		IF(num = 1)
			result := 1;
		ELSE
            IF(num >1)
                result := factorial(num); 
            ELSE
                result := 0;
            ENDIF
		ENDIF
		WRITE(result,eol);
		RETURN 0;
	END
END
```

more can be found in the test_files folder
