/*
 * Parser kompilatora "głupiego", JFTT2022
 *
 * Autor: Maciek Józefkowicz
 * 10.01.2023
*/

%{

#include <iostream>

#include "logic.cpp"
using namespace std;

extern int yylineno;
int yylex( void );
void yyset_in( FILE* in_str);
void yyerror(char const* s);

%union {
  long long l_val;
  string str_val;
}

%}

%token NUM
%token IDENTIFIER
%token PROCEDURE
%token IS
%token VAR
%token BEGIN
%token END
%token PROGRAM
%token IF
%token THEN
%token ELSE
%token ENDIF
%token WHILE
%token REPEAT
%token DO
%token ENDWHILE
%token READ
%token WRITE
%token UNTIL
%token LTEQ	// <=
%token GTEQ	// >=
%token LT	// <
%token GT	// >
%token UNEQ	// !=
%token SET 	// :=
%token SEMI 	// ;
%token PLUS	// +
%token MINUS	// -
%token TIMES	// *
%token MOD	// %
%token DIV	// /
%token COMMA	// ,

%%

program_all:
procedures main
;

procedures:
%empty
| procedures PROCEDURE proc_head IS VAR declarations BEGIN commands END
| procedures PROCEDURE proc_head IS BEGIN commands END
;

main:
PROGRAM IS VAR declarations BEGIN commands END
| PROGRAM IS BEGIN commands END
;

commands:
commands command
| command
;

command:
IDENTIFIER SET expression ';'
| IF condition THEN commands ELSE commands ENDIF
| IF condition THEN commands ENDIF
| WHILE condition DO commands ENDWHILE
| REPEAT commands UNTIL condition ';'
| proc_head ';'
| READ IDENTIFIER ';'
| WRITE value ';'
;

proc_head:
IDENTIFIER '(' declarations ')'
;

declarations:
declarations ',' IDENTIFIER
| IDENTIFIER

expression:
value
| value '+' value
| value '-' value
| value '*' value
| value '/' value
| value '%' value
;

condition:
value '=' value
| value UNEQ value
| value GT value
| value LT value
| value GTEQ value
| value LTEQ value
;

value:
NUM
| IDENTIFIER

%%

void yyerror(char const *s){
  cerr<< s << "in line " << yylineno << endl;
}

void run(FILE* data, FILE* out){
  cout << "Kompilacja" << endl;
  yyset_in(data);
  yyparse();
}