/* 
 * parser.y - Parser utility for the DHBW compiler
 */
 
%{	
	// Project-specific includes
	#include "diag.h"
  #include "uthash.h"
  #include <string.h>

  typedef struct paramstruct {
    int paramNr;
    char* type;
    char* name;
    struct paramstruct* next;
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
    char* type; //INT="1"; VOID="2"
    int paramcount;
    STRUCTPARAM* funcparams;//array
    UT_hash_handle hh;
  } STRUCTFUNC;
  int var_exists(char* id);
  int func_exists(char* func_id);
  void add_var(char *id, char *type, int value);
  void add_func(char* id, char* type, int numberOfParams);
  void log_vars();
  void log_funcs();
  void yyerror (const char *msg);
  void push_something();
  void message_logger(char* msg);
  STRUCTVAR *variables = NULL;
  STACK* programstack;
  STRUCTFUNC* functions = NULL;
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
%type <i> function_parameter_list

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
     : type ID PARA_OPEN PARA_CLOSE {add_func($2,$1,0);}
     | type ID PARA_OPEN function_parameter_list PARA_CLOSE {add_func($2,$1,$4);}
     ;

function_parameter_list
     : function_parameter {$$=1;}
     | function_parameter_list COMMA function_parameter {$$=$1+1;}
     ;
	
function_parameter
     : type identifier_declaration {push(&programstack,$1);push(&programstack,$2);}
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
  printf("Stack is: %s\n",temp);
  peek(programstack,&temp);
}

void add_var(char *id, char *type, int value){
  STRUCTVAR *s;
  s = (STRUCTVAR*)malloc(sizeof(STRUCTVAR));
  s->id = (char*)malloc(sizeof(id));
  s->type = (char*)malloc(sizeof(type));
  strcpy(s->id, id);
  strcpy(s->type, type);
  s->value = value;
  HASH_ADD_INT(variables,id,s);
  log_vars();
}

void add_func(char* id, char* type, int numberOfParams){
  if(!func_exists(id)){
    STRUCTFUNC *s;
    s = (STRUCTFUNC*)malloc(sizeof(STRUCTFUNC));
    s->id =(char*)malloc(sizeof(id));
    s->type = (char*)malloc(sizeof(type));
    strcpy(s->id,id);
    strcpy(s->type,type);
    s->paramcount = numberOfParams;
    //s->funcparams = (STRUCTPARAM*)malloc(sizeof(STRUCTPARAM));
    STRUCTPARAM *p = NULL;
    STRUCTPARAM *tempstruct;
    char* temptype;
    char* tempid;
    while(numberOfParams>0){
      pop(&programstack,&tempid);
      pop(&programstack,&temptype);
      tempstruct = p;
      p = (STRUCTPARAM*)malloc(sizeof(STRUCTPARAM));
      p->type = (char*)malloc(sizeof(temptype));
      p->name = (char*)malloc(sizeof(tempid));
      strcpy(p->type,temptype);
      strcpy(p->name,tempid);
      p->paramNr = numberOfParams;
      p->next = tempstruct;
      numberOfParams--;
    }
    s->funcparams = p;
    HASH_ADD_INT(functions,id,s);
    log_funcs();
  }else{
    message_logger("Function already exists!");
  }
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

int func_exists(char* func_id){
  STRUCTFUNC *temp;
  for(temp = functions; temp!=NULL;temp=temp->hh.next){
    if(!strcmp(temp->id,func_id)){
      break;
    }
  }
  if(temp!=NULL)
    return 1;//printf("Test: %s %s\n",temp->id,temp->type);
  return 0;
}

void log_vars(){
  STRUCTVAR *temp;
  printf("\n\nVariables-Table looks as following:\n\n");
  for(temp = variables; temp!=NULL;temp=temp->hh.next){
    printf("Variable-Entry: id: %s, type: %s, value: %d\n",temp->id,temp->type,temp->value);
  }
}

void log_funcs(){
  STRUCTFUNC *temp;
  printf("\n\nFunctiontypes are INT=1 and VOID=2");
  printf("\n\nFunction-Table looks as following:\n\n");
  for(temp = functions; temp!=NULL;temp=temp->hh.next){
    printf("Function-Entry: id: %s, type: %s, paramcount: %d\n",temp->id,temp->type,temp->paramcount);
    if(temp->funcparams!=NULL){
      STRUCTPARAM* tempparam = temp->funcparams;
      while(tempparam!=NULL){
        printf("\tParameter for this function: paramNr: %d type: %s name: %s\n",tempparam->paramNr,tempparam->type,tempparam->name);
        tempparam=tempparam->next;
      }
    }
  }
}

void message_logger(char* msg){
  printf("Following Message is sent:\n(%d.%d-%d.%d): %s\n", yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, msg);
}