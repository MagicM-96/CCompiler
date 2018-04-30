#pragma once
#include "structs.h"

void messageLogger(char* msg);
void errorLogger(char* msg0, char* msg1, char* msg2, ERRORLINEINFO* errorLineInfo);
void lineColLogger(int firstLine, int firstColumn, int lastLine, int lastColumn);

// error Handler
void throwIfStatementError(ERRORLINEINFO* errorLineInfo);
void throwWhileLoopError(ERRORLINEINFO* errorLineInfo);
void throwAssignmentError(ERRORLINEINFO* errorLineInfo);
void throwLogCompError(ERRORLINEINFO* errorLineInfo);
void throwMatchOpError(ERRORLINEINFO* errorLineInfo);
void throwShiftOpError(ERRORLINEINFO* errorLineInfo);