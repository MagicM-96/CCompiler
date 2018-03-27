//Author: Patrick

#include "output.h"

extern STRUCTVAR *variables;
extern STRUCTFUNC *functions;

STRUCTVAR *tempVars;
STRUCTFUNC *tempFuncs;

void printSymTable() {
    printf("\n\nLegend: INT = 1; VOID = 2; INT-ARRAY = 3\n\n");
    // print variables-subtable
    printf("Variables:\n");
    printf("\tid\ttype\tvalue\n");
    for(tempVars = variables; tempVars != NULL; tempVars = tempVars->hh.next){
        printf("\t%s\t%s\t%d\n", tempVars->id,tempVars->type,tempVars->value);
    }
    // print functions-subtable
    printf("\nFunctions:\n");
    for(tempFuncs = functions; tempFuncs != NULL;tempFuncs = tempFuncs->hh.next){
    printf("Function: id: %s, type: %s, paramcount: %d\n",tempFuncs->id,tempFuncs->type,tempFuncs->paramcount);
    // print parameter-subsubtable
    if(tempFuncs->funcparams!=NULL){
      STRUCTPARAM *tempParam = tempFuncs->funcparams;
      printf("\tParameters:\n");
      printf("\tnumber\ttype\tname\n");
      while(tempParam!=NULL){
        printf("\t%d\t%s\t%s\n",tempParam->paramNr,tempParam->type,tempParam->name);
        tempParam = tempParam->next;
      }
    }
  }
}

/*  
    Debug-Functions.
    For debug purposes only!
*/

void log_vars(){
    printf("\n\nVariables-Table looks as following:\n\n");
    for(tempVars = variables; tempVars != NULL; tempVars = tempVars->hh.next){
        printf("Variable-Entry: \n\tid: %s\n\ttype: %s\n\tvalue: %d\n\tsize: %d\n",tempVars->id,tempVars->type,tempVars->value,tempVars->size);
    }
}

void log_funcs(){
  printf("\n\nFunctiontypes are INT=1 and VOID=2 and INT-ARRAY=3");
  printf("\n\nFunction-Table looks as following:\n\n");
  for(tempFuncs = functions; tempFuncs != NULL; tempFuncs = tempFuncs->hh.next){
    printf("Function-Entry: id: %s, type: %s, paramcount: %d\n",tempFuncs->id,tempFuncs->type,tempFuncs->paramcount);
    if(tempFuncs->funcparams!=NULL){
      STRUCTPARAM* tempParam = tempFuncs->funcparams;
      while(tempParam!=NULL){
        printf("\tParameter for this function: paramNr: %d type: %s name: %s length: %d\n",tempParam->paramNr,tempParam->type,tempParam->name,tempParam->size);
        tempParam = tempParam->next;
      }
    }
  }
}