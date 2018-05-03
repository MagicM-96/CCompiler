/* 
 * scanner.l - Scanner utility for the DHBW compiler
 */

%{
	#include "parser.y.h"

	int column = 0;
	#define YY_USER_ACTION {\
	yylloc.first_line = yylineno; \
	yylloc.last_line = yylineno; \
	yylloc.first_column = column; \
	yylloc.last_column = column + yyleng-1; \
	column += yyleng;}
%}

/* 
 * Default setting for flex which indicates that yytext, which is one of the main connections
 * between the scanner and parser, is handled as pointer of char instead of an array of char. 
*/
%pointer	

/* save the line number of terminals */
%option yylineno

/* finish after scanning the provided file */
%option noyywrap		

%%
\n {column = 0;}

do { return DO; }
while { return WHILE; }
if { return IF; }
else { return ELSE; }
int { return INT; }
void {return VOID; }
return { return RETURN; }

0|[1-9]+[0-9]* { yylval.i = atoi(yytext); return NUM; } 
[_a-zA-Z][-_a-zA-Z0-9]* { yylval.id = strdup(yytext); return ID; }

[:] { return COLON; }
[,] { return COMMA; }
[;] { return SEMICOLON; }
\{ { return BRACE_OPEN; }
\} { return BRACE_CLOSE; }
\[ { return BRACKET_OPEN; }
\] { return BRACKET_CLOSE; }
\( { return PARA_OPEN; }
\) { return PARA_CLOSE; }
[=] { return ASSIGN; }
\|\| { return LOGICAL_OR; }
&& { return LOGICAL_AND; }
== { return EQ; }
!= { return NE; }
\< { return LS; }
\> { return GT; }
[<][=] { return LSEQ; }
[>][=] { return GTEQ; }
[<][<] { return SHIFT_LEFT; }
[>][>] { return SHIFT_RIGHT; }
\+ { return PLUS; }
\- { return MINUS; }
\* { return MUL; }
\/ { return DIV; }
% { return MOD; }
! { return LOGICAL_NOT; }


%%

