/* 
 * parser.y - Parser utility for the DHBW compiler
 */
 
%{	
#include "diag.h"
#include "uthash.h"
#include "structs.h"
#include "stack.h"
#include "output.h"
#include "symboltable/funcSymboltable.h"
  #include "globalVar.h"
  #include <string.h>

  // Project-specific includes
int yylex();
  int varExists(char* id);
  int funcExists(char* funcId);
void messageLogger(char* msg);
  void defineFunc(char* id, char* type, int numberOfParams);
  void yyerror (const char *msg);
  void identifierDeclaration(int length, char* type);
  void pushSomething();
  void typeReplace(char** type);
  void addVariablesToFunction(char* id);
  void endScope();
  void startScope();
  void addVar(char *id, char *type, int value, int size);
  void logVars();
  void logFuncs();
  extern void yyerror (const char *msg);
  void pushSomething();
  STRUCTVAR *variables = NULL;
  SCOPESTACK* scopes = NULL;
  extern STACK* programstack;
  extern STRUCTFUNC* functions;
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
%type <i> identifierDeclaration
%type <id> type
%type <i> functionParameterList

%%

program
     : { startScope(); } programElementList {endScope(); printSymTable();}
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
     : type ID PARA_OPEN PARA_CLOSE BRACE_OPEN { startScope(); } stmtList BRACE_CLOSE {defineFunc($2,$1,0);endScope();}
     | type ID PARA_OPEN functionParameterList PARA_CLOSE BRACE_OPEN { startScope(); } stmtList BRACE_CLOSE{defineFunc($2,$1,$4);endScope();}
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
     | RETURN expression SEMICOLON
     | RETURN SEMICOLON
     | SEMICOLON /* empty statement */
     ;

stmtBlock
     : BRACE_OPEN stmtList BRACE_CLOSE
     ;
	
stmtConditional
     : IF PARA_OPEN expression PARA_CLOSE stmt
     | IF PARA_OPEN expression PARA_CLOSE stmt ELSE stmt
     ;
									
stmtLoop
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
     | functionCall
     | primary 
     ;

primary
     : NUM {$$=$1;}
     | ID {$$=$1;}
     ;

functionCall
      : ID PARA_OPEN PARA_CLOSE
      | ID PARA_OPEN functionCallParameters PARA_CLOSE
      ;

functionCallParameters
     : functionCallParameters COMMA expression
     | expression
     ;

%%

void yyerror (const char *msg)
{
	FATAL_COMPILER_ERROR(INVALID_SYNTAX, 0, "(%d.%d-%d.%d): %s\n", yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, msg);
}


  
void identifierDeclaration(int length, char* type){
  if(!strcmp(type,"0")){
    char* tempid;
    char* temptype;
    pop(&programstack,&tempid);
    peek(programstack,&temptype);
    if(strcmp(temptype,"VOID")){
      if(!varExists(tempid)){
        if(length==1)
          addVar(tempid,"INT",0,length);
        else
          addVar(tempid,"INT-ARR",0,length);
      }else{
        printf("Variable %s already exists!",tempid);
      }
    }
  }else{
    if(!strcmp(type,"VOID"))
      messageLogger("Can't declare variable as void!");
    else{ 
      char* id;
      pop(&programstack,&id);
      push(&programstack,type);
      if(!varExists(id)){
        if(length==1)
          addVar(id,"INT",0,length);
        else
          addVar(id,"INT-ARR",0,length);
      }else{
        printf("Variable %s already exists!",id);
      }
    }
  }
}

void pushSomething(){
  push(&programstack,"test1");
  char* temp;
  peek(programstack,&temp);
  pop(&programstack,&temp);
  printf("Stack is: %s\n",temp);
  peek(programstack,&temp);
}

void typeReplace(char** type){ //Noch nicht implementiert, kommt vlt. noch
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

void defineFunc(char* id, char* type, int numberOfParams){
  if(funcExists(id)){//Here implementation of parameter checking
    addVariablesToFunction(id);
  }else{
    addFunc(id,type,numberOfParams);
    functions->funcvars = variables;
  }
}

void addVar(char *id, char *type, int value, int size){
  STRUCTVAR *s;
  s = (STRUCTVAR*)malloc(sizeof(STRUCTVAR));
  s->id = (char*)malloc(sizeof(id));
  s->type = (char*)malloc(sizeof(type));
  strcpy(s->id, id);
  strcpy(s->type, type);
  s->value = value;
  s->size = size;
  HASH_ADD_INT(variables,id,s);
  //logVars();
}

void startScope(){
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

void endScope(){
  if(scopes!=NULL){
    variables = scopes->scope;
    SCOPESTACK* temp;
    temp = scopes->next;
    scopes = temp;
  }
}



void addVariablesToFunction(char* id){
  STRUCTFUNC* temp = functions;
  while(strcmp(temp->id,id)){
    temp = temp->hh.next;
  }
  temp->funcvars = variables;
}

int varExists(char* varId){
  STRUCTVAR *temp;
  for(temp = variables; temp!=NULL;temp=temp->hh.next){
    if(!strcmp(temp->id,varId)){
      break;
    }
  }
  if(temp!=NULL)
    return 1;
  return 0;
}

int funcExists(char* funcId){
  STRUCTFUNC *temp;
  for(temp = functions; temp!=NULL;temp=temp->hh.next){
    if(!strcmp(temp->id,funcId)){
      break;
    }
  }
  if(temp!=NULL)
    return 1;
  return 0;
}
void messageLogger(char* msg){
  printf("Following Message is sent:\n(%d.%d-%d.%d): %s\n", yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, msg);
}
