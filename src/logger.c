#include "logger.h"
// #include "structs.h"
#include <stdio.h>
extern int errors;

void messageLogger(char* msg)
{
	// TODO @Benedikt please fix this
	errors++;
	printf("There is something not implemented yet, please ask your Developer to do so");
	// printf("Following Message is sent:\n(%d.%d-%d.%d): %s\n", yylloc.first_line, yylloc.first_column,
	// yylloc.last_line, yylloc.last_column, msg);
}

void errorLogger(char* msg0, char* msg1, char* msg2, ERRORLINEINFO* errorLineInfo)
// This is a bad way to handle this, but the other ways inside c are even more crappy
{
	// TODO @Benedikt please fix this
	errors++;
	printf("\n|> ERROR: %s%s%s\n|> START: %d : %d --> END: %d : %d", msg0, msg1, msg2, errorLineInfo->firstLine,
		errorLineInfo->firstColumn, errorLineInfo->lastLine, errorLineInfo->lastColumn);
	// printf("Following Message is sent:\n(%d.%d-%d.%d): %s\n", yylloc.first_line, yylloc.first_column,
	// yylloc.last_line, yylloc.last_column, msg);
}

// Functions for most of the Errors that could possibly be thrown.

void throwIfStatementError(ERRORLINEINFO* errorLineInfo)
{
	errorLogger("If-Statement", ":", "Incompatible variable type!", errorLineInfo);
}

void throwWhileLoopError(ERRORLINEINFO* errorLineInfo)
{
	errorLogger("While-loop", ": ", "Incompatible variable type!", errorLineInfo);
}

void throwAssignmentError(ERRORLINEINFO* errorLineInfo)
{
	errorLogger("Assignment", ": ", "Incompatible variable type!", errorLineInfo);
}

void throwLogCompError(ERRORLINEINFO* errorLineInfo)
{
	errorLogger("Logical comparison", ": ", "Incompatible variable type!", errorLineInfo);
}

void throwMatchOpError(ERRORLINEINFO* errorLineInfo)
{
	errorLogger("Math Operation", ": ", "Incompatible variable type!", errorLineInfo);
}

void throwShiftOpError(ERRORLINEINFO* errorLineInfo)
{
	errorLogger("Shift Operation", ": ", "Incompatible variable type!", errorLineInfo);
}