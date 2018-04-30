#include "tempcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int tempVars = 0;
int endif = 0;
int endelse = 0;
int endwhile = 0;
int funcs = 0;
TEMPCODESTRING* tempCode, *firstTempCode;


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
    switch(opcode){
        case OPASSIGN:
            sprintf(temp,"%s = %s;",op1,op2);
            strcpy(returnVal,"1");
            tempVars++;
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
    while(temp->line!=NULL){
        printf("%s\n",temp->line);
        temp = temp->next;
    }
    tempCode = NULL;
}
