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
"+"             { return PLUS;  }
"-"             { return MINUS; }
"*"             { return TIMES; }
"/"             { return DIV;   }
"%"             { return MOD;   }
"<="            { return  LTEQ;}
">="            { return  GTEQ;}
"<"             { return  LT;}
">"             { return  GT;}
"!="            { return  UNEQ;}
":="            { return  SET;}
;               { return SEMI;}
,               { return COMMA;}

[_a-z]+         { yylval.str_val = yytext; return identifier; }
[0-9]+          { yylval.l_val = atoll( yytext );  return num; }
.               { yyerror("illegal symbol");}

%%