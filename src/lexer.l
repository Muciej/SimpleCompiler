/*
 *  Lexer kompilatora "głupiego", JFTT 2022
 *
 *  Autor: Maciek Józefkowicz
 *  10.01.2023
 *
*/

%option noyywrap
%option yylineno
%option nounput

%{
#include "parser.hh"
int yylex();
%}

%%

\[.*\] ;
[0-9]+          { yylval = atoll( yytext );  return num; };

%%