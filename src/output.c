//Author: Patrick

#include "output.h"

extern STRUCTVAR *variables;
extern STRUCTFUNC *functions;

STRUCTVAR *tempVars;
STRUCTFUNC *tempFuncs;

void printSymTable() {
    printf("-----Symboltable---------------------------------------------------\n");
    // print variables-subtable
    printf("Variables:\n");
    if (variables == NULL) {
        printf("\tNo global variables were found.\n");
    } else {        
        printf("\tid\t\ttype\t\tvalue\n");
        for(tempVars = variables; tempVars != NULL; tempVars = tempVars->hh.next){
            printf("\t%s\t\t%s\t\t%d", tempVars->id,tempVars->type,tempVars->value);
            if (tempVars->size > 1) {
                printf("\tarray-size: %d", tempVars->size);
            }
            printf("\n");
        }
    }
    
    // print functions-subtable
    printf("\nFunctions:");
    if (functions == NULL) {
        printf("\tNo functions were found.\n");
    } else {
        for(tempFuncs = functions; tempFuncs != NULL;tempFuncs = tempFuncs->hh.next){
            printf("\n");
            printf("- Function: id: %s, type: %s, paramcount: %d\n",tempFuncs->id,tempFuncs->type,tempFuncs->paramcount);
            // print parameter-subsubtable
            if(tempFuncs->funcparams!=NULL){
                STRUCTPARAM *tempParam = tempFuncs->funcparams;
                printf("\tParameters:\n");
                printf("\t|number\t\ttype\t\tname\n");
                while(tempParam!=NULL){
                    printf("\t|%d\t\t%s\t\t%s",tempParam->paramNr,tempParam->type,tempParam->name);
                    if(tempParam->size > 1) {
                        printf("\tarray-size: %d", tempParam->size);
                    }
                    printf("\n");
                    tempParam = tempParam->next;
                }
            }
            printf("\n");
            if (tempFuncs->funcvars != NULL) {
                STRUCTVAR *tempInnerVars = tempFuncs->funcvars;
                printf("\tInner variables:\n");
                printf("\t|id\t\ttype\t\tvalue\n");
                for(; tempInnerVars != NULL; tempInnerVars = tempInnerVars->hh.next){
                    printf("\t|%s\t\t%s\t\t%d", tempInnerVars->id,tempInnerVars->type,tempInnerVars->value);
                    if (tempInnerVars->size > 1) {
                        printf("\tarray-size: %d", tempInnerVars->size);
                    }
                    printf("\n");
                }
            }
        }
    }
    printf("-------------------------------------------------------------------\n");
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