/*
 *  Lexer kompilatora "głupiego", JFTT 2022
 *
 *  Autor: Maciek Józefkowicz
 *
*/

%option noyywrap
%option yylineno
%option nounput

%{
#include "parser.hpp"
%}

%%
\[.*\]          ;
[ \t\n]         ;
[_a-z]+         { yylval.str_val = yytext; return IDENTIFIER; }
[0-9]+          { yylval.l_val = atoll( yytext ); return NUM; }

PROCEDURE       { return KW_PROCEDURE; }
IS              { return  KW_IS;}
VAR             { return  KW_VAR;}
BEGIN           { return  KW_BEGIN;}
END             { return  KW_END;}
PROGRAM         { return  KW_PROGRAM;}
IF              { return  KW_IF;}
THEN            { return  KW_THEN;}
ELSE            { return  KW_ELSE;}
ENDIF           { return  KW_ENDIF;}
WHILE           { return  KW_WHILE;}
REPEAT          { return  KW_REPEAT;}
DO              { return  KW_DO;}
ENDWHILE        { return  KW_ENDWHILE;}
READ            { return  KW_READ;}
WRITE           { return  KW_WRITE;}
UNTIL           { return  KW_UNTIL;}
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

EOF             { return KW_EOF; }
.               {}

%%