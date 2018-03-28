#include "funcSymboltable.h"

  #include "../diag.h"
  #include "../uthash.h"
  #include "../structs.h"
  #include "../stack.h"
  #include "../output.h"
  #include <string.h>


void add_func(char* id, char* type, int numberOfParams){
  if(!func_exists(id)){
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
    message_logger("Function already exists!");
  }
}