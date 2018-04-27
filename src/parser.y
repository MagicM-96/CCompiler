/* 
 * parser.y - Parser utility for the DHBW compiler
 */
 
%code requires {	
  #include "checker.h"
  #include "logger.h"
  #include "diag.h"
  #include "output.h"
  #include "symboltable.h"
  #include "stack.h"
  #include "structs.h"
  #include "tempcode.h"

  // Project-specific includes
  void yyerror (const char *msg);
  extern STACK* programstack;
  extern STRUCTFUNC* functions;
}

%union {
  int i;
  char *id;
  TYPEVAR tv;
  
}
 
// Verbose error messages
%error-verbose

%locations
%start program

// Compiler in debugging information
%debug

// Enable debug output
%initial-action
{
	yydebug = 0;
};

/*
 * One shift/reduce conflict is expected for the "dangling-else" problem. This
 * conflict however is solved by the default behavior of bison for shift/reduce 
 * conflicts (shift action). The default behavior of bison corresponds to what
 * we want bison to do: SHIFT if the lookahead is 'ELSE' in order to bind the 'ELSE' to
 * the last open if-clause. 
 */
%expect 1

%token DO WHILE
%token IF ELSE
%token INT VOID
%token RETURN

%token COLON COMMA SEMICOLON

%token BRACE_OPEN BRACE_CLOSE
%token BRACKET_OPEN BRACKET_CLOSE
%token PARA_OPEN PARA_CLOSE

%token <id>ID
%token <i> NUM

%token ASSIGN 
%token LOGICAL_OR
%token LOGICAL_AND
%token EQ NE     
%token LS LSEQ GTEQ GT 
%token SHIFT_LEFT SHIFT_RIGHT
%token PLUS MINUS     
%token MUL DIV MOD
%token LOGICAL_NOT UNARY_MINUS UNARY_PLUS

%right ASSIGN
%left LOGICAL_AND LOGICAL_OR
%left LS GT LSEQ GTEQ EQ NE
%left SHIFT_LEFT SHIFT_RIGHT
%left PLUS MINUS
%left MUL DIV MOD
%left LOGICAL_NOT UNARY_MINUS UNARY_PLUS

%type <id>  primary
%type <i>   identifierDeclaration
%type <id>  type
%type <i>   functionParameterList
%type <id>  expression
%type <tv>  functionCall
%type <i>   functionCallParameters

%%

program
     : programElementList { printSymTable();char temp[4]; addCode(OPADD,&temp,"A","B","C");}
     ;

programElementList
     : programElementList programElement 
     | programElement 
     ;

programElement
     : variableDeclaration SEMICOLON
     | functionDeclaration SEMICOLON
     | functionDefinition
     | SEMICOLON
     ;

type
     : INT {$$="INT";}
     | VOID {$$="VOID";}
     ;

variableDeclaration
     : variableDeclaration COMMA identifierDeclaration  {identifierDeclaration($3,"0");}
     | type identifierDeclaration {identifierDeclaration($2,$1);}
     ;

identifierDeclaration
     : ID BRACKET_OPEN NUM BRACKET_CLOSE {push(&programstack,$1);$$=$3;}
     | ID {push(&programstack,$1);$$=1;}
     ;

functionDefinition
     : type ID PARA_OPEN PARA_CLOSE BRACE_OPEN { if(!funcExists($2)) addFunc($2,$1,0); startScope(); } stmtList BRACE_CLOSE {defineFunc($2,$1,0);endScope();}
     | type ID PARA_OPEN functionParameterList PARA_CLOSE BRACE_OPEN { if(!funcExists($2)) addFunc($2,$1,$4); startScope(); } stmtList BRACE_CLOSE{defineFunc($2,$1,$4);endScope();}
     ;

functionDeclaration
     : type ID PARA_OPEN PARA_CLOSE {addFunc($2,$1,0);}
     | type ID PARA_OPEN functionParameterList PARA_CLOSE {addFunc($2,$1,$4);}
     ;

functionParameterList
     : functionParameter {$$=1;}
     | functionParameterList COMMA functionParameter {$$=$1+1;}
     ;
	
functionParameter
     : type identifierDeclaration {push(&programstack,$1);char temp[3];sprintf(temp,"%d",$2);push(&programstack,temp);}
     ;
									
stmtList
     : /* empty: epsilon */
     | stmtList stmt
     ;

stmt
     : stmtBlock
     | variableDeclaration SEMICOLON
     | expression SEMICOLON
     | stmtConditional
     | stmtLoop
     | RETURN expression SEMICOLON  {addVar("functionsReturnParameter",$2,0,1);}
     | RETURN SEMICOLON {addVar("functionsReturnParameter","VOID",0,1);}
     | SEMICOLON /* empty statement */
     ;

stmtBlock
     : BRACE_OPEN stmtList BRACE_CLOSE
     ;
	
stmtConditional
     : IF PARA_OPEN expression PARA_CLOSE stmt            {if(!isInt($3)) {errorLogger("If-Statement",":","Incompatible variable type!");};}
     | IF PARA_OPEN expression PARA_CLOSE stmt ELSE stmt  {if(!isInt($3)) {errorLogger("If-Statement", ": ", "Incompatible variable type!");};}
     ;
									
stmtLoop
     : WHILE PARA_OPEN expression PARA_CLOSE stmt         {if(!isInt($3)) {errorLogger("While-loop", ": ", "Incompatible variable type!");};}
     | DO stmt WHILE PARA_OPEN expression PARA_CLOSE SEMICOLON  {if(!isInt($5)) {errorLogger("While-loop", ": ", "Incompatible variable type!");};}
     ;
									
expression
     : expression ASSIGN expression       {if(isTypeCompatible($1, $3)){$$="INT";} else {errorLogger("Assignment", ": ", "Incompatible variable type!");};}
     | expression LOGICAL_OR expression   {if(isTypeCompatible($1, $3)){$$="INT";} else {errorLogger("Logical comparison", ": ", "Incompatible variable type!");};}
     | expression LOGICAL_AND expression  {if(isTypeCompatible($1, $3)){$$="INT";} else {errorLogger("Logical comparison", ": ", "Incompatible variable type!");};}
     | LOGICAL_NOT expression {if(isTypeCompatible($2, "INT")){$$="INT";} else {errorLogger("Logical Not", ": ", "Incompatible variable type!");};}
     | expression EQ expression   {if(isTypeCompatible($1, $3)){$$="INT";} else {errorLogger("Logical comparison", ": ", "Incompatible variable type!");};}
     | expression NE expression   {if(isTypeCompatible($1, $3)){$$="INT";} else {errorLogger("Logical comparison", ": ", "Incompatible variable type!");};}
     | expression LS expression   {if(isTypeCompatible($1, $3)){$$="INT";} else {errorLogger("Logical comparison", ": ", "Incompatible variable type!");};}
     | expression LSEQ expression {if(isTypeCompatible($1, $3)){$$="INT";} else {errorLogger("Comparison", ": ", "Incompatible variable type!");};}
     | expression GTEQ expression {if(isTypeCompatible($1, $3)){$$="INT";} else {errorLogger("Comparison", ": ", "Incompatible variable type!");};}
     | expression GT expression   {if(isTypeCompatible($1, $3)){$$="INT";} else {errorLogger("Comparison", ": ", "Incompatible variable type!");};}
     | expression PLUS expression   {if(isTypeCompatible($1, $3)){$$="INT";} else {errorLogger("Math Operation", ": ", "Incompatible variable type!");};}
     | expression MINUS expression  {if(isTypeCompatible($1, $3)){$$="INT";} else {errorLogger("Math Operation", ": ", "Incompatible variable type!");};}
     | expression MUL expression    {if(isTypeCompatible($1, $3)){$$="INT";} else {errorLogger("Math Operation", ": ", "Incompatible variable type!");};}
     | expression DIV expression    {if(isTypeCompatible($1, $3)){$$="INT";} else {errorLogger("Math Operation", ": ", "Incompatible variable type!");};}
     | expression SHIFT_LEFT expression   {if(isTypeCompatible($1, $3)){$$="INT";} else {errorLogger("Shift Operation", ": ", "Incompatible variable type!");};}
     | expression SHIFT_RIGHT expression  {if(isTypeCompatible($1, $3)){$$="INT";} else {errorLogger("Shift Operation", ": ", "Incompatible variable type!");};}
     | MINUS expression %prec UNARY_MINUS {$$=$2;}
     | PLUS expression %prec UNARY_PLUS {$$=$2;}
     | ID BRACKET_OPEN primary BRACKET_CLOSE  {if(checkVarType($1,"INT-ARR",1)){$$="INT";}else{errorLogger("Type-Error : Variable \"",$1,"\" is not an Array!\n");}}
     | PARA_OPEN expression PARA_CLOSE  {$$=$2;}
     | functionCall {$$=$1.type;}
     | primary  {$$=$1;}
     ;

primary
     : NUM {$$="INT";}
     | ID {char* temp; lookupVariableType($1,&temp);$$=temp;}
     ;

functionCall
      : ID PARA_OPEN PARA_CLOSE {checkFuncCallParams($1,0); char* temp; lookupFunctionType($1,&temp);$$.type=temp;}
      | ID PARA_OPEN functionCallParameters PARA_CLOSE {checkFuncCallParams($1,$3); char* temp; lookupFunctionType($1,&temp);$$.type=temp;}
      ;

functionCallParameters
     : functionCallParameters COMMA expression  {push(&programstack, $3);$$=$1+1;}
     | expression {push(&programstack,$1);$$=1;}
     ;

%%

void yyerror (const char *msg)
{
	FATAL_COMPILER_ERROR(INVALID_SYNTAX, 0, "(%d.%d-%d.%d): %s\n", yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, msg);
}
