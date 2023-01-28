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
procedures main		{
				logic.print_debug( yylineno );
 				logic.println_debug(" procedures main");
			}
;

procedures:
  %empty
| procedures KW_PROCEDURE proc_head KW_IS KW_VAR { logic.set_def(true); } declarations {logic.set_def(false);} KW_BEGIN commands KW_END {
											logic.print_debug(yylineno);
											logic.println_debug(" Procedure with vars");
											logic.handle_proc_end();
										       }
| procedures KW_PROCEDURE proc_head KW_IS KW_BEGIN commands KW_END {
									logic.print_debug(yylineno);
									logic.println_debug(" Procedure without vars");
									logic.handle_proc_end();
								   }
;

main:
KW_PROGRAM KW_IS KW_VAR {logic.start_main(); logic.set_def(true); } declarations { logic.set_def(false); } KW_BEGIN commands KW_END {
								logic.print_debug(yylineno);
								logic.println_debug(" Main with vars");

							      }
| KW_PROGRAM KW_IS {logic.start_main(); } KW_BEGIN commands KW_END {
						logic.print_debug(yylineno);
						logic.println_debug( " Main without vars");
				    	    }
;

commands:
commands command	{
				logic.print_debug(yylineno);
				logic.println_debug(" Commands list");
			}
| command	{
			logic.print_debug(yylineno);
			logic.println_debug(" Command");

		}
;

command:
IDENTIFIER SET expression SEMI		{
						logic.print_debug( yylineno );
						logic.println_debug(" Ident: " + $1 + " set exp ");
						logic.handle_expression($3);
						logic.handle_var_usage($1, true);
						logic.handle_set_comm();
					}
| KW_IF condition KW_THEN commands KW_ELSE {logic.handle_else(); } commands KW_ENDIF {
								logic.print_debug(yylineno);
								logic.println_debug(" If then else");
								logic.handle_ifelse();
							     }
| KW_IF condition KW_THEN commands KW_ENDIF	{
							logic.print_debug(yylineno);
							logic.println_debug(" If then");
							logic.handle_if();
						}
| KW_WHILE condition {logic.depth_inc(); } KW_DO commands KW_ENDWHILE	{
							logic.print_debug(yylineno);
							logic.println_debug(" While loop");
							logic.depth_dec();
							logic.handle_while();
						}
| KW_REPEAT {logic.depth_inc(); logic.handle_repeat_begin(); } commands KW_UNTIL condition SEMI	{
							logic.print_debug(yylineno);
							logic.println_debug(" Unitl loop");
							logic.depth_dec();
							logic.handle_repeat_end();
						}
| proc_head SEMI			{
						logic.print_debug(yylineno);
						logic.println_debug(" proc_head");
					}
| KW_READ IDENTIFIER SEMI		{
						logic.print_debug(yylineno);
						logic.println_debug(" Read ident:" +  $2 );
						logic.handle_var_usage($2, true);
						logic.handle_read_comm();
					}
| KW_WRITE value SEMI			{
						logic.print_debug(yylineno);
						logic.println_debug(" Write val");
						logic.handle_write_comm();
					}
;

proc_head:
IDENTIFIER L_PAR declarations R_PAR	{
						logic.print_debug( yylineno );
  						logic.println_debug(" ident: " + $1 + " with declarations");
  						logic.handle_proc_head($1);

					}
;

declarations:
declarations COMMA IDENTIFIER		{
						logic.print_debug( yylineno );
						logic.println_debug(" Decl: Ident: " + $3);
						logic.handle_var_decl($3);

					}
| IDENTIFIER				{
						logic.print_debug( yylineno );
						logic.println_debug(" Decl: Ident: "  + $1);
						logic.handle_var_decl($1);
					}
;

expression:
value					{
						logic.print_debug(yylineno);
						logic.println_debug(" Value " + $1);
						$$ = "just_var";
					}
| value PLUS value			{
						logic.print_debug(yylineno);
						logic.println_debug(" Value + Value");
						$$ = "+";
					}
| value MINUS value			{
						logic.print_debug(yylineno);
						logic.println_debug(" Value - Value");
						$$ = "-";
					}
| value TIMES value			{
						logic.print_debug(yylineno);
						logic.println_debug(" Value * Value");
						$$ = "*";
					}
| value DIV value			{
						logic.print_debug(yylineno);
						logic.println_debug(" Value / Value ");
						$$ = "/";
					}
| value MOD value			{
						logic.print_debug(yylineno);
						logic.println_debug(" Value % Value ");
						$$ = "%";
					}
;

condition:
value EQ value				{
						logic.print_debug(yylineno);
						logic.println_debug(" Value = Value");
						logic.handle_condition("==");
					}
| value UNEQ value			{
						logic.print_debug(yylineno);
						logic.println_debug(" Value != Value");
						logic.handle_condition("!=");
					}
| value GT value			{
						logic.print_debug(yylineno);
						logic.println_debug(" Value > Value");
						logic.handle_condition(">");
					}
| value LT value			{
						logic.print_debug(yylineno);
						logic.println_debug(" Value < Value");
						logic.handle_condition("<");
					}
| value GTEQ value			{
						logic.print_debug(yylineno);
						logic.println_debug(" Value >= Value");
						logic.handle_condition(">=");
					}
| value LTEQ value			{
						logic.print_debug(yylineno);
						logic.println_debug(" Value <= Value");
						logic.handle_condition("<=");
					}
;

value:
NUM					{
						logic.print_debug(yylineno);
						logic.println_debug(" Number: " + $1);
						logic.handle_const_usage($1);
					}
| IDENTIFIER				{
						logic.print_debug(yylineno);
						logic.println_debug(" Ident: " + $1);
						logic.handle_var_usage($1);
					}
;

%%

void yyerror(char const *s){
  cerr<< s << " in line " << yylineno << endl;
  logic.print_debug();
  exit(1);
}

void run(FILE* data, ofstream& out, ofstream& debugst){
  cout << "Kompilacja" << endl;
  yyset_in(data);
  //outFile = std::move(out);
  logic.outFile = std::move(out);
  if(debugst.good()){
  	cout<<"Debug mode"<<endl;
  	logic.debug = true;
  	logic.debugFile = std::move(debugst);
  }
  yyparse();
  logic.give_mem_address();
  logic.to_assembly();
  logic.d_print_program_structures();
  logic.d_print_var_stack();
  logic.print_program_structure();
  logic.close();
}