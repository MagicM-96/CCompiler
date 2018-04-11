#pragma once

#include "string.h"

int isTypeCompatible(char* leftVarType, char* rightVarType);
int isInt(char* varType);
void checkReturnParam(char* id,char* type);
int checkFuncParams(char* id, int numberOfParams);
int varExists(char* id, int allScopes);
int funcExists(char* funcId);
int funcIsDefined(char* funcId);
int checkFuncType(char* id, char* type);
int checkVarType(char* id, char* type, int allScopes);