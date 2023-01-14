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

\[.*\]          ;
[ \t\n]         ;

PROCEDURE       { return PROCEDURE; }
IS              { return  IS;}
VAR             { return  VAR;}
BEGIN           { return  BEGIN;}
END             { return  END;}
PROGRAM         { return  PROGRAM;}
IF              { return  IF;}
THEN            { return  THEN;}
ELSE            { return  ELSE;}
ENDIF           { return  ENDIF;}
WHILE           { return  WHILE;}
REPEAT          { return  REPEAT;}
DO              { return  DO;}
ENDWHILE        { return  ENDWHILE;}
READ            { return  READ;}
WRITE           { return  WRITE;}
UNTIL           { return  UNTIL;}
"<="            { return  lteq;}
">="            { return  gteq;}
"<"             { return  lt;}
">"             { return  gt;}
"!="            { return  uneq;}
":="            { return  set;}
;               { return yytext[0];}
,               { return yytext[0];}

[_a-z]+         { return identifier; }
[0-9]+          { yylval = atoll( yytext );  return num; }
.               { yyerror("illegal word");}

%%