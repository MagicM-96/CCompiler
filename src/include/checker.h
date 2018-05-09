#pragma once
#include "structs.h"

int isTypeCompatible(char* leftVarType, char* rightVarType);
int isInt(char* varType);

int varExists(char* id, int allScopes);
int funcExists(char* funcId);
int funcIsDefined(char* funcId);

void checkReturnParam(char* id, char* type, ERRORLINEINFO* errorLineInfo);
void checkFuncCallParams(char* funcId, int numberOfParams, ERRORLINEINFO* errorLineInfo);
int checkFuncParams(char* id, int numberOfParams, ERRORLINEINFO* errorLineInfo);

int checkFuncType(char* id, char* type);
int checkVarType(char* id, char* type, int allScopes);
