/*
 * Parser kompilatora "głupiego", JFTT2022
 *
 * Autor: Maciek Józefkowicz
*/
%{

#include <iostream>
#include <fstream>
#include "logic.cpp"

using namespace std;

Logic logic;
extern int yylineno;
ofstream debugstream;
ofstream outFile;
int debug = 0;
int yylex( void );
void yyset_in( FILE* in_str);
void yyerror(char const* s);

%}

%define api.value.type{std::string}

%token IDENTIFIER
%token NUM
%token KW_PROGRAM
%token KW_PROCEDURE
%token KW_IS
%token KW_BEGIN
%token KW_VAR
%token KW_END
%token KW_IF
%token KW_THEN
%token KW_ELSE
%token KW_ENDIF
%token KW_WHILE
%token KW_DO
%token KW_ENDWHILE
%token KW_REPEAT
%token KW_READ
%token KW_WRITE
%token KW_UNTIL
%token EQ	// =
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
%token L_PAR	// (
%token R_PAR	// )

%%

program_all:
procedures main		{	if(debug) debugstream<<  yylineno<< " procedures main"<<endl;

			}
;

procedures:
procedures KW_PROCEDURE proc_head KW_IS KW_VAR declarations KW_BEGIN commands KW_END {if(debug) debugstream<< yylineno<< " Procedure with vars"<<endl;}
| procedures KW_PROCEDURE proc_head KW_IS KW_BEGIN commands KW_END {if(debug) debugstream<< yylineno<< " Procedure without vars"<<endl;}
| %empty
;

main:
KW_PROGRAM KW_IS KW_VAR declarations KW_BEGIN commands KW_END {if(debug) debugstream<< yylineno<< " Main with vars"<<endl;}
| KW_PROGRAM KW_IS KW_BEGIN commands KW_END {if(debug) debugstream<< yylineno<< " Main without vars"<<endl;}
;

commands:
commands command	{if(debug) debugstream<< yylineno<< " Commands list"<<endl;}
| command	{
			if(debug) debugstream<< yylineno<< " Command"<<endl;

		}
;

command:
IDENTIFIER SET expression SEMI		{if(debug) debugstream<<yylineno<<" Ident: " << $1 << " set exp "<<endl;}
| KW_IF condition KW_THEN commands KW_ELSE commands KW_ENDIF {if(debug) debugstream<<yylineno<<" If then else"<<endl;}
| KW_IF condition KW_THEN commands KW_ENDIF	{if(debug) debugstream<<yylineno<<" If then"<<endl;}
| KW_WHILE condition KW_DO commands KW_ENDWHILE	{ if(debug) debugstream<<yylineno<<" While loop"<<endl;}
| KW_REPEAT commands KW_UNTIL condition SEMI	{ if(debug) debugstream<<yylineno<<" Unitl loop"<<endl;}
| proc_head SEMI			{if(debug) debugstream << yylineno<< " proc_head"<<endl; }
| KW_READ IDENTIFIER SEMI		{if(debug) debugstream << yylineno<< " Read ident: " << $2 << endl;}
| KW_WRITE value SEMI			{if(debug) debugstream << yylineno<< " Write val"<<endl; }
;

proc_head:
IDENTIFIER L_PAR declarations R_PAR	{	if(debug) debugstream << yylineno<<  " ident: " << $1 << " with declarations"<< endl;

					}
;

declarations:
declarations COMMA IDENTIFIER		{	if(debug) debugstream << yylineno<< " , Ident: "<< $3 << endl;}
| IDENTIFIER				{	if(debug) debugstream <<  yylineno<< " Ident: " << $1 << endl;

					}
;

expression:
value					{if(debug) debugstream<< yylineno<< " Value "<< $1 <<endl;}
| value PLUS value			{
						if(debug) debugstream<< yylineno<< " Value + Value"<<endl;
					}
| value MINUS value			{
						if(debug) debugstream<< yylineno<< " Value - Value"<<endl;
					}
| value TIMES value			{if(debug) debugstream<< yylineno<< " Value * Value"<<endl;}
| value DIV value			{if(debug) debugstream<< yylineno<< " Value / Value "<<endl;}
| value MOD value			{if(debug) debugstream<< yylineno<< " Value % Value "<<endl;}
;

condition:
value EQ value				{if(debug) debugstream<< yylineno<< " Value = Value"<<endl;}
| value UNEQ value			{if(debug) debugstream<< yylineno<< " Value != Value"<<endl;}
| value GT value			{if(debug) debugstream<< yylineno<< " Value > Value"<<endl;}
| value LT value			{if(debug) debugstream<< yylineno<< " Value < Value"<<endl;}
| value GTEQ value			{if(debug) debugstream<< yylineno<< " Value >= Value"<<endl;}
| value LTEQ value			{if(debug) debugstream<< yylineno<< " Value <= Value"<<endl;}
;

value:
NUM					{if(debug) debugstream<< yylineno<< " Number: "<< $1 <<endl;}
| IDENTIFIER				{if(debug) debugstream<< yylineno<< " Ident: "<<$1 <<endl;}
;

%%

void yyerror(char const *s){
  cerr<< s << " in line " << yylineno << endl;
}

void run(FILE* data, ofstream& out, ofstream& debugst){
  cout << "Kompilacja" << endl;
  yyset_in(data);
  //outFile = std::move(out);
  logic.outFile = std::move(out);
  if(debugst.good()){
  	cout<<"Debug mode"<<endl;
  	debug = 1;
  	debugstream = std::move(debugst);
  }
  yyparse();
  out.close();
}