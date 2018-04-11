#pragma once

void addFunc(char* id, char* type, int numberOfParams);
int yylex();	   // TODO is this necessary?

void defineFunc(char* id, char* type, int numberOfParams);

void identifierDeclaration(int length, char* type);
void pushSomething();
void typeReplace(char** type);
void addVariablesToFunction(char* id);
void endScope();
void startScope();
void addVar(char* id, char* type, int value, int size);
void logVars();
void logFuncs();
extern void yyerror(const char* msg);
void pushSomething();

void lookupFunctionType(char* funcId, char** ret);
void lookupVariableType(char* varId, char** ret);
