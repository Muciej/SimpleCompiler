/*
 * Parser kompilatora "głupiego", JFTT2022
 *
 * Autor: Maciek Józefkowicz
 * 10.01.2023
*/

%{

#define YYSTYPE long long

#include <iostream>
#include "logic.cpp"
using namespace std;

extern int yylineno;
int yylex( void );
void yyset_in( FILE* in_str);
void yyerror(char const* s);

%}

%token num
%token identifier

%%

program_all:
procedures main
;

procedures:
%empty
| procedures "PROCEDURE" proc_head "IS VAR" declarations "BEGIN" commands "END"
| procedures "PROCEDURE" proc_head "IS BEGIN" commands "END"
;

main:
"PROGRAM IS VAR" declarations "BEGIN" commands "END"
| "PROGRAM IS BEGIN" commands "END"
;

commands:
commands command
| command
;

command:
identifier ":=" expression ";"
| "IF" condition "THEN" commands "ELSE" commands "ENDIF"
| "IF" condition "THEN" commands "ENDIF"
| "WHILE" condition "DO" commands "ENDWHILE"
| "REPEAT" commands "UNTIL" condition ";"
| proc_head ";"
| "READ" identifier ";"
| "WRITE" value ";"
;

proc_head:
identifier "(" declarations ")"
;

declarations:
declarations "," identifier
| identifier

expression:
value
| value "+" value
| value "-" value
| value "*" value
| value "/" value
| value "%" value
;

condition:
value "=" value
| value "!=" value
| value ">" value
| value "<" value
| value ">=" value
| value "<=" value
;

value:
num
| identifier

%%

void yyerror(char const *s){
  cerr<< s << "in line " <<  << endl;
}

void run(FILE* data, FILE* out){
  cout << "Kompilacja" << endl;
  yyset_in(data);
  yyparse();
}