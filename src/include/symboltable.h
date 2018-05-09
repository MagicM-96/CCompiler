#pragma once
#include "structs.h"

void addFunc(char* id, char* type, int numberOfParams, ERRORLINEINFO* errorLineInfo);
void addVar(char* id, char* type, int value, int size);

void defineFunc(char* id, char* type, int numberOfParams, ERRORLINEINFO* errorLineInfo, char* label);
void addVariablesToFunction(char* id, char* label);

void startScope();
void endScope();

void identifierDeclaration(int length, char* type, ERRORLINEINFO* errorLineInfo);

void lookupFunctionType(char* funcId, char** ret);
void lookupFunctionLabel(char* funcId, char** ret);
void lookupVariableType(char* varId, char** ret);
