#include "tempcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logger.h"


typedef struct Vars{
    char* tempVar[4];
    char* varName;
    struct Vars* next;
}TEMPVARS;

int tempVars = 0;
int endif = 0;
int endelse = 0;
int endwhile = 0;
int funcs = 0;
TEMPCODESTRING* tempCode, *firstTempCode;
TEMPVARS *firstVar,*lastVar, *foundVar;


/**
 * @brief Function writes tempCode and saves it in a char*.
 * 
 * @param opcode Opcode to create (see enum in header file)
 * @param ret1 Parameter where the returning string is written on
 * @param op1 operand 1 for operation (required/null if not existing)
 * @param op2 operand 2 for operation (required/null if not existing)
 * @param op3 operand 3 for operation (required/null if not existing)
 */
void addCode(int opcode, char** ret1, char* op1, char* op2, char* op3){
	char temp[100];
    char returnVal[4];
    switch(opcode)
    {
        case OPASSIGN:
            if(isVariable(op1,&returnVal))
            {
                sprintf(temp,"%s = %s;",foundVar->varName,op2);
                tempVars++;
            }
            else
            {
                //errorLogger("Type-Error: Expression ist not a variable!","","");
            }
            break;
        case OPADD:
            sprintf(temp,"V%d = %s + %s",tempVars,op1,op2);
            sprintf(returnVal,"V%d",tempVars);
            tempVars++;
            break;
        case OPSUB:
            sprintf(temp,"V%d = %s - %s",tempVars,op1,op2);
            sprintf(returnVal,"V%d",tempVars);
            tempVars++;
            break;
        case CREATEVAR:
            sprintf(temp,"%s = %s",op1,op2);
            strcpy(returnVal,op1);
            break;
        default:
            strcpy(temp,"Unknown OPCODE!\n");
            strcpy(returnVal,"-1");

    }
    (*ret1) = returnVal;
    //sprintf(temp,"%d is %d with %s = %s + %s",opcode,OPADD,op1,op2,op3);
    addStr(temp);
    printStr();
    printf("Returns: %s\n",returnVal);
}

void createVar(char* id, char* type,char** ret)
{
    if(firstVar==NULL)
    {
        firstVar = (TEMPVARS*)malloc(sizeof(TEMPVARS));
        lastVar = firstVar;
    }
    lastVar->varName = (char*)malloc(sizeof(id));
    strcpy(lastVar->varName,id);
    sprintf(lastVar->tempVar,"V%d",tempVars);
    tempVars++;
    addCode(CREATEVAR,ret,lastVar->tempVar,lastVar->varName,NULL);
    lastVar->next = (TEMPVARS*)malloc(sizeof(TEMPVARS));
    lastVar = lastVar->next;
    //logTempVars();
}

void addStr(char* str)
{
    if(tempCode==NULL)
    {
        tempCode = (TEMPCODESTRING*)malloc(sizeof(TEMPCODESTRING));
        firstTempCode = tempCode;
    }
    tempCode->next = (TEMPCODESTRING*)malloc(sizeof(TEMPCODESTRING));
    tempCode->line = (char*)malloc(sizeof(str));
    strcpy(tempCode->line,str);
    tempCode = tempCode->next;
}

void printStr()
{
    TEMPCODESTRING *temp = firstTempCode;
    while(temp->line!=NULL)
    {
        printf("%s\n",temp->line);
        temp = temp->next;
    }
    tempCode = NULL;
}

int isVariable(char* var, char** ret)
{
    TEMPVARS* temp;
    temp = firstVar;
    while(temp != NULL && temp->tempVar != NULL)
    {
        if(!strcmp(var, temp->tempVar))
        {
            (*ret) = "1";
            foundVar = temp;
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

void logTempVars()
{
    TEMPVARS* temp;
    temp = firstVar;
    while(temp != NULL && temp->tempVar != NULL)
    {
        printf("Variable: ID: %s, TEMP: %s\n",temp->varName,temp->tempVar);
        temp = temp->next;
    }
}
