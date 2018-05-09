#pragma once
#include "structs.h"

void printSymTable();

void errorLogger(char* msg0, char* msg1, char* msg2, ERRORLINEINFO* errorLineInfo);

// error Handler
void throwIfStatementError(ERRORLINEINFO* errorLineInfo);
void throwWhileLoopError(ERRORLINEINFO* errorLineInfo);
void throwAssignmentError(ERRORLINEINFO* errorLineInfo);
void throwLogCompError(ERRORLINEINFO* errorLineInfo);
void throwMathOpError(ERRORLINEINFO* errorLineInfo);
void throwShiftOpError(ERRORLINEINFO* errorLineInfo);

//printing Tempcode
void printStr();
