#include "parserFuncs.h"

#include "diag.h"
#include "uthash.h"
#include "structs.h"
#include "stack.h"
#include "output.h"
#include "symboltable/funcSymboltable.h"
#include <string.h>
//THIS FILE IS ONLY TEMPORARILY AND SHOULD BE DELETED AS SOON AS POSSIBLE
STACK* programstack;
STRUCTFUNC* functions = NULL;
STRUCTVAR *variables = NULL;
SCOPESTACK* scopes = NULL;


void addFunc(char* id, char* type, int numberOfParams){
  if(!funcExists(id)){
    STRUCTFUNC *s;
    s = (STRUCTFUNC*)malloc(sizeof(STRUCTFUNC));
    s->id =(char*)malloc(sizeof(id));
    s->type = (char*)malloc(sizeof(type));
    strcpy(s->id,id);
    strcpy(s->type,type);
    s->paramcount = numberOfParams;
    STRUCTPARAM *p = NULL;
    STRUCTPARAM *tempstruct;
    char* temptype;
    char* tempid;
    char* templength;
    while(numberOfParams>0){
      pop(&programstack,&templength);
      pop(&programstack,&temptype);
      pop(&programstack,&tempid);
      if(atoi(templength)>1){
        strcat(temptype,"-ARR");
      }
      tempstruct = p;
      p = (STRUCTPARAM*)malloc(sizeof(STRUCTPARAM));
      p->type = (char*)malloc(sizeof(temptype));
      p->name = (char*)malloc(sizeof(tempid));
      strcpy(p->type,temptype);
      strcpy(p->name,tempid);
      p->paramNr = numberOfParams;
      p->size = atoi(templength);
      p->next = tempstruct;
      numberOfParams--;
    }
    s->funcparams = p;
    HASH_ADD_INT(functions,id,s);
    //log_funcs();
  }else{
    messageLogger("Function already exists!");
  }
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
  //TODO @Benedikt please fix this
  printf("there is something not implemented jet, please ask your Developer to do so");
  //printf("Following Message is sent:\n(%d.%d-%d.%d): %s\n", yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, msg);
}
