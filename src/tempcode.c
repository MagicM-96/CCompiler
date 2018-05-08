#include "tempcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logger.h"
#include "structs.h"


int globTempVars = 0;
int endif = 0;
int endwhile = 0;
int funcs = 0;
TEMPCODESTRING *tempCode, *firstTempCode;
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
            sprintf(temp,"%s = %s;",foundVar->varName,op2);
            strcpy(returnVal,op2);
            break;
        case OPADD:
            sprintf(temp,"V%d = %s + %s;",globTempVars,op1,op2);
            sprintf(returnVal,"V%d",globTempVars);
            globTempVars++;
            break;
        case OPSUB:
            sprintf(temp,"V%d = %s - %s;",globTempVars,op1,op2);
            sprintf(returnVal,"V%d",globTempVars);
            globTempVars++;
            break;
		case OPMUL:
			sprintf(temp,"V%d = %s * %s;",globTempVars,op1,op2);
            sprintf(returnVal,"V%d",globTempVars);
            globTempVars++;
			break;
		case OPDIV:
			sprintf(temp,"V%d = %s / %s;",globTempVars,op1,op2);
            sprintf(returnVal,"V%d",globTempVars);
            globTempVars++;
			break;
		case OPMINUS:
			sprintf(temp,"V%d = -%s;",globTempVars,op1); //TODO maybe try to load 0 into a variable and do 0 - op1
            sprintf(returnVal,"V%d",globTempVars);
			globTempVars++;
			break;
        case OPNOT:
            sprintf(temp,"V%d = !%s;",globTempVars,op1);
            sprintf(returnVal,"V%d",globTempVars);
			globTempVars++;
            break;
		case OPLSHIFT:
			sprintf(temp,"V%d = %s << %s;",globTempVars,op1,op2);
            sprintf(returnVal,"V%d",globTempVars);
			globTempVars++;
			break;
		case OPRSHIFT:
			sprintf(temp,"V%d = %s >> %s;",globTempVars,op1,op2);
            sprintf(returnVal,"V%d",globTempVars);
			globTempVars++;
			break;
		case OPIFEQ:
			sprintf(temp,"V%d = %s == %s;",globTempVars,op1,op2);
            sprintf(returnVal,"V%d",globTempVars);
			globTempVars++;
			break;
		case OPIFNE:
			sprintf(temp,"V%d = %s != %s;",globTempVars,op1,op2);
            sprintf(returnVal,"V%d",globTempVars);
			globTempVars++;
			break;
		case OPIFGT:
			sprintf(temp,"V%d = %s > %s;",globTempVars,op1,op2);
            sprintf(returnVal,"V%d",globTempVars);
			globTempVars++;
			break;
		case OPIFGE:
			sprintf(temp,"V%d = %s >= %s;",globTempVars,op1,op2);
            sprintf(returnVal,"V%d",globTempVars);
			globTempVars++;
			break;
		case OPIFLT:
			sprintf(temp,"V%d = %s < %s;",globTempVars,op1,op2);
            sprintf(returnVal,"V%d",globTempVars);
			globTempVars++;
			break;
		case OPIFLE:
			sprintf(temp,"V%d = %s <= %s;",globTempVars,op1,op2);
            sprintf(returnVal,"V%d",globTempVars);
			globTempVars++;
			break;
		case OPGOTO:
			break;
		case OPRETURNR:
            sprintf(temp,"return %s;",op1);
			break;
		case OPRETURN:
            strcpy(temp,"return;");
			break;
		case OPCALLR:
            if(op2==NULL)
            {
                sprintf(temp,"V%d=%s();",globTempVars,op1);
            }
            else
            {
                sprintf(temp,"V%d=%s(%s);",globTempVars,op1,op2);
            }
            sprintf(returnVal,"V%d",globTempVars);
            globTempVars++;
			break;
		case OPCALL:
            if(op2==NULL)
            {
                sprintf(temp,"%s();",op1);
            }
            else
            {
                sprintf(temp,"%s(%s);",op1,op2);
            }
            sprintf(returnVal,"");
			break;
		case OPARRAY_LD:
			break;
		case OPARRAY_ST:
			break;		
		case OPOR:
			sprintf(temp,"V%d = %s || %s;",globTempVars,op1,op2);
			sprintf(returnVal,"V%d",globTempVars);
            globTempVars++;
			break;
		case OPAND:
			sprintf(temp,"V%d = %s && %s;",globTempVars,op1,op2);
			sprintf(returnVal,"V%d",globTempVars);
            globTempVars++;
			break;
        case CREATEVAR:
            sprintf(temp,"%s = %s;",op1,op2);
            strcpy(returnVal,op1);
            break;
        case BEGINIF:
            sprintf(temp,"if(%s) goto STARTIF%d;\ngoto ENDIF%d;\nSTARTIF%d:",op1,endif,endif,endif);
            sprintf(returnVal,"%d",endif);
            endif++;
            break;
        case ENDIF:
            sprintf(temp,"ENDIF%s:",op1);
            break;
        case BEFOREELSE:
            sprintf(temp,"goto ENDELSE%s;\nENDIF%s:",op1,op1);
            break;
        case AFTERELSE:
            sprintf(temp,"ENDELSE%s:",op1);
            break;
        case STARTWHILE:
            sprintf(temp,"BEGINWHILE%s:",op1);
            strcpy(returnVal,op1);
            break;
        case CHECKWHILE:
            sprintf(temp,"CHECKWHILE%s:",op1);
            strcpy(returnVal,op1);
            break;
        case CHECKIFWHILE:
            sprintf(temp,"if(%s) goto BEGINWHILE%s;\ngoto ENDWHILE%s;",op2,op1,op1);
            strcpy(returnVal,op1);
            break;
        case ENDWHILE:
            sprintf(temp,"goto CHECKWHILE%s;\nENDWHILE%s:",op1,op1);
            strcpy(returnVal,op1);
            break;
        case STARTFUNC:
            sprintf(temp,"STARTFUNC%d:",funcs);
            sprintf(returnVal,"%d",funcs);
            funcs++;
            break;
        case FUNCPARAM:
            sprintf(temp,"V%d = %s;",globTempVars,op1);
            sprintf(returnVal,"V%d",globTempVars);
            globTempVars++;
            break;
        
        default:
            strcpy(temp,"Unknown OPCODE!\n");
            strcpy(returnVal,"-1");

    }
    char* tempRet = (char*)malloc(sizeof(returnVal));
    strcpy(tempRet,returnVal);
    if(ret1!=NULL)
        (*ret1) = tempRet;
    //sprintf(temp,"%d is %d with %s = %s + %s",opcode,OPADD,op1,op2,op3);
    addStr(temp);
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
    sprintf(lastVar->tempVar,"V%d",globTempVars);
    globTempVars++;
    addCode(CREATEVAR,ret,lastVar->tempVar,lastVar->varName,NULL);
    lastVar->next = (TEMPVARS*)malloc(sizeof(TEMPVARS));
    lastVar = lastVar->next;
    //logTempVars();
}

void createArr(char* id, char* num, char* type, char** ret)
{
    sprintf(id,"%s[%s]",id,num);
    createVar(id,type,ret);
}

void loadNum(int val, char** ret)
{
    char* op1=(char*)malloc(sizeof(char)*4);
    char* op2=(char*)malloc(sizeof(char)*4);
    sprintf(op1,"%d",val);
    sprintf(op2,"V%d",globTempVars);
    globTempVars++;
    addCode(CREATEVAR,ret,op2,op1,NULL);
}

void addStr(char* str)
{
    if(firstTempCode==NULL)
    {
        tempCode = (TEMPCODESTRING*)malloc(sizeof(TEMPCODESTRING));
        firstTempCode = tempCode;
    }
    tempCode->next = (TEMPCODESTRING*)malloc(sizeof(TEMPCODESTRING));
    tempCode->line = (char*)malloc(sizeof(str)*100);
    strcpy(tempCode->line,str);
    tempCode = tempCode->next;
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

void getLoopNumber(char** ret)
{
    char* temp = (char*)malloc(sizeof(char)*4);
    sprintf(temp,"%d",endwhile);
    endwhile++;
    (*ret) = temp;
}
