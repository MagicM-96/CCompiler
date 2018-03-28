/* 
 * parser.y - Parser utility for the DHBW compiler
 */
 
%{	
	// Project-specific includes
	#include "diag.h"
  #include "uthash.h"
  #include "structs.h"
  #include "stack.h"
  #include "output.h"
  #include "symboltable/funcSymboltable.h"
  #include <string.h>
  int yylex();
  int var_exists(char* id);
  int func_exists(char* func_id);

  void yyerror (const char *msg);
  void identifierdeclaration(int length, char* type);
  void push_something();
  void type_replace(char** type);
  void define_func(char* id, char* type, int numberOfParams);
  void add_variables_to_function(char* id);
  void end_scope();
  void start_scope();
  void add_var(char *id, char *type, int value, int size);
  void log_vars();
  void log_funcs();
  void yyerror (const char *msg);
  void push_something();
  void message_logger(char* msg);
  STRUCTVAR *variables = NULL;
  SCOPESTACK* scopes = NULL;
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
%type <i> identifier_declaration
%type <id> type
%type <i> function_parameter_list

%%

program
     : { start_scope(); } program_element_list {end_scope(); printSymTable();}
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
     : INT {$$="INT";}
     | VOID {$$="VOID";}
     ;

variable_declaration
     : variable_declaration COMMA identifier_declaration  {identifierdeclaration($3,"0");}
     | type identifier_declaration {identifierdeclaration($2,$1);}
     ;

identifier_declaration
     : ID BRACKET_OPEN NUM BRACKET_CLOSE {push(&programstack,$1);$$=$3;}
     | ID {push(&programstack,$1);$$=1;}
     ;

function_definition
     : type ID PARA_OPEN PARA_CLOSE BRACE_OPEN { start_scope(); } stmt_list BRACE_CLOSE {define_func($2,$1,0);end_scope();}
     | type ID PARA_OPEN function_parameter_list PARA_CLOSE BRACE_OPEN { start_scope(); } stmt_list BRACE_CLOSE{define_func($2,$1,$4);end_scope();}
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
     : type identifier_declaration {push(&programstack,$1);char temp[3];sprintf(temp,"%d",$2);push(&programstack,temp);}
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
     | primary 
     ;

primary
     : NUM {$$=$1;}
     | ID {$$=$1;}
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

void identifierdeclaration(int length, char* type){
  if(!strcmp(type,"0")){
    char* tempid;
    char* temptype;
    pop(&programstack,&tempid);
    peek(programstack,&temptype);
    if(strcmp(temptype,"VOID")){
      if(!var_exists(tempid)){
        if(length==1)
          add_var(tempid,"INT",0,length);
        else
          add_var(tempid,"INT-ARR",0,length);
      }else{
        printf("Variable %s already exists!",tempid);
      }
    }
  }else{
    if(!strcmp(type,"VOID"))
      message_logger("Can't declare variable as void!");
    else{ 
      char* id;
      pop(&programstack,&id);
      push(&programstack,type);
      if(!var_exists(id)){
        if(length==1)
          add_var(id,"INT",0,length);
        else
          add_var(id,"INT-ARR",0,length);
      }else{
        printf("Variable %s already exists!",id);
      }
    }
  }
}

void push_something(){
  push(&programstack,"test1");
  char* temp;
  peek(programstack,&temp);
  pop(&programstack,&temp);
  printf("Stack is: %s\n",temp);
  peek(programstack,&temp);
}

void type_replace(char** type){ //Noch nicht implementiert, kommt vlt. noch
  if(!strcmp((*type),"0")){
    printf("\nType is undefined!\n\n");
  }else if(!strcmp((*type),"1")){
    printf("\nType is INT!\n\n");
  }else if(!strcmp((*type),"2")){
    printf("\nType is VOID!\n\n");
  }else if(!strcmp((*type),"3")){
    printf("\nType is INT-ARR!\n\n");
  }
}

void add_var(char *id, char *type, int value, int size){
  STRUCTVAR *s;
  s = (STRUCTVAR*)malloc(sizeof(STRUCTVAR));
  s->id = (char*)malloc(sizeof(id));
  s->type = (char*)malloc(sizeof(type));
  strcpy(s->id, id);
  strcpy(s->type, type);
  s->value = value;
  s->size = size;
  HASH_ADD_INT(variables,id,s);
  //log_vars();
}

void start_scope(){
  if (variables != NULL){
    //push variables on scopestack
    SCOPESTACK* temp;
    temp = (SCOPESTACK*)malloc(sizeof(SCOPESTACK));
    temp->scope = variables;
    temp->next = scopes;
    scopes = temp;
    variables = NULL;
  }
}

void end_scope(){
  if(scopes!=NULL){
    variables = scopes->scope;
    SCOPESTACK* temp;
    temp = scopes->next;
    scopes = temp;
  }
}

void define_func(char* id, char* type, int numberOfParams){
  if(func_exists(id)){//Here implementation of parameter checking
    add_variables_to_function(id);
  }else{
    add_func(id,type,numberOfParams);
    functions->funcvars = variables;
  }
}

void add_variables_to_function(char* id){
  STRUCTFUNC* temp = functions;
  while(strcmp(temp->id,id)){
    temp = temp->hh.next;
  }
  temp->funcvars = variables;
}


int var_exists(char* var_id){
  STRUCTVAR *temp;
  for(temp = variables; temp!=NULL;temp=temp->hh.next){
    if(!strcmp(temp->id,var_id)){
      break;
    }
  }
  if(temp!=NULL)
    return 1;
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
    return 1;
  return 0;
}


void message_logger(char* msg){
  printf("Following Message is sent:\n(%d.%d-%d.%d): %s\n", yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, msg);
}