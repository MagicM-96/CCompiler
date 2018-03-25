/* 
 * parser.y - Parser utility for the DHBW compiler
 */
 
%{	
	// Project-specific includes
	#include "diag.h"
  #include "uthash.h"
  #include <string.h>

  typedef struct paramstruct {
    char* type;
  } STRUCTPARAM;
  typedef struct StackNode {
    char* data;
    struct StackNode* next;
  } STACK;
  typedef struct varstruct {
    char* id;
    char* type;
    char* size;
    int value;
    UT_hash_handle hh;
  } STRUCTVAR;
  typedef struct funcstruct {
    char* id;
    char* type;
    STRUCTPARAM* funcparams;//array
    STRUCTPARAM returnparam;
    UT_hash_handle hh;
  } STRUCTFUNC;
  int var_exists(char* id);
  void add_var(char *id, char *type, int value);
  void log_struct();
  void message_logger(char* msg);
  STRUCTVAR *variables = NULL;
  STACK* programstack;
%}

%union {
  int i;
  char *id;
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

%type <id> primary
%type <id> identifier_declaration
%type <id> type

%%

program
     : program_element_list
     ;

program_element_list
     : program_element_list program_element 
     | program_element 
     ;

program_element
     : variable_declaration SEMICOLON
     | function_declaration SEMICOLON
     | function_definition
     | SEMICOLON
     ;
									
type
     : INT {$$="1";}
     | VOID {$$="2";}
     ;

variable_declaration
     : variable_declaration COMMA identifier_declaration  {
        char* temp;
        peek(programstack,&temp);
        if(strcmp(temp,"2")){
          if(!var_exists($3)){
            printf("Now add variable %s\n",$3);
            add_var($3,"int",0);
          }else{
            printf("Variable %s already exists!",$3);
          }
        }
      }
     | type identifier_declaration {
        if(!strcmp($1,"2"))
          message_logger("Can't declare variable as void!");
        else{ 
          printf("Pushing type on stack : %s\n",$1);
          push(&programstack,$1);
          if(!var_exists($2)){
            printf("Now add variable %s\n",$2);
            add_var($2,"int",0);
          }else{
            printf("Variable %s already exists!",$2);
          }
        }
      }
     ;

identifier_declaration
     : ID BRACKET_OPEN NUM BRACKET_CLOSE
     | ID {
        //add_var($1,"int","0");log_struct();yylval.id = $1;var_exists("test1");push_something();
        $$=$1;
      }
     ;

function_definition
     : type ID PARA_OPEN PARA_CLOSE BRACE_OPEN stmt_list BRACE_CLOSE
     | type ID PARA_OPEN function_parameter_list PARA_CLOSE BRACE_OPEN stmt_list BRACE_CLOSE
     ;

function_declaration
     : type ID PARA_OPEN PARA_CLOSE
     | type ID PARA_OPEN function_parameter_list PARA_CLOSE
     ;

function_parameter_list
     : function_parameter
     | function_parameter_list COMMA function_parameter
     ;
	
function_parameter
     : type identifier_declaration
     ;
									
stmt_list
     : /* empty: epsilon */
     | stmt_list stmt
     ;

stmt
     : stmt_block
     | variable_declaration SEMICOLON
     | expression SEMICOLON
     | stmt_conditional
     | stmt_loop
     | RETURN expression SEMICOLON
     | RETURN SEMICOLON
     | SEMICOLON /* empty statement */
     ;

stmt_block
     : BRACE_OPEN stmt_list BRACE_CLOSE
     ;
	
stmt_conditional
     : IF PARA_OPEN expression PARA_CLOSE stmt
     | IF PARA_OPEN expression PARA_CLOSE stmt ELSE stmt
     ;
									
stmt_loop
     : WHILE PARA_OPEN expression PARA_CLOSE stmt
     | DO stmt WHILE PARA_OPEN expression PARA_CLOSE SEMICOLON
     ;
									
expression
     : expression ASSIGN expression
     | expression LOGICAL_OR expression
     | expression LOGICAL_AND expression
     | LOGICAL_NOT expression
     | expression EQ expression
     | expression NE expression
     | expression LS expression 
     | expression LSEQ expression 
     | expression GTEQ expression 
     | expression GT expression
     | expression PLUS expression
     | expression MINUS expression
     | expression SHIFT_LEFT expression
     | expression SHIFT_RIGHT expression
     | expression MUL expression
     | expression DIV expression
     | MINUS expression %prec UNARY_MINUS
     | PLUS expression %prec UNARY_PLUS
     | ID BRACKET_OPEN primary BRACKET_CLOSE
     | PARA_OPEN expression PARA_CLOSE
     | function_call
     | primary  {printf("test\ntest\ntest\ntest\ntest\ntest\ntest\ntest\ntest\n");printf("From primary I got : %s\n",$1);}
     ;

primary
     : NUM {printf("Parsed : %d\n", $1);$$=$1;}
     | ID {printf("Parsed : %s\n", $1);$$=$1;}
     ;

function_call
      : ID PARA_OPEN PARA_CLOSE
      | ID PARA_OPEN function_call_parameters PARA_CLOSE
      ;

function_call_parameters
     : function_call_parameters COMMA expression
     | expression
     ;

%%

void yyerror (const char *msg)
{
	FATAL_COMPILER_ERROR(INVALID_SYNTAX, 0, "(%d.%d-%d.%d): %s\n", yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, msg);
}

void push_something(){
  push(&programstack,"test1");
  char* temp;
  peek(programstack,&temp);
  pop(&programstack,&temp);
  peek(programstack,&temp);
}

void add_var(char *id, char *type, int value){
  STRUCTVAR *s;
  s = (STRUCTVAR*)malloc(sizeof(STRUCTVAR));
  s->id = (char*)malloc(sizeof(id));
  s->type = (char*)malloc(sizeof(type));
  s->value = (int*)malloc(sizeof(value));
  strcpy(s->id, id);
  strcpy(s->type, type);
  s->value = value;
  HASH_ADD_INT(variables,id,s);
  log_struct();
}

int var_exists(char* var_id){
  STRUCTVAR *temp;
  for(temp = variables; temp!=NULL;temp=temp->hh.next){
    if(!strcmp(temp->id,var_id)){
      break;
    }
  }
  if(temp!=NULL)
    return 1;//printf("Test: %s %s\n",temp->id,temp->type);
  return 0;
}

void log_struct(){
  STRUCTVAR *temp;
  for(temp = variables; temp!=NULL;temp=temp->hh.next){
    printf("Entry: id: %s, type: %s, value: %d\n",temp->id,temp->type,temp->value);
  }
}

void message_logger(char* msg){
  printf("Following Message is sent:\n(%d.%d-%d.%d): %s\n", yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, msg);
}