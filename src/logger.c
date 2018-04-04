#include "logger.h"
extern int errors;

void messageLogger(char* msg)
{
	// TODO @Benedikt please fix this
	errors++;
	printf("there is something not implemented jet, please ask your Developer to do so");
	// printf("Following Message is sent:\n(%d.%d-%d.%d): %s\n", yylloc.first_line, yylloc.first_column,
	// yylloc.last_line, yylloc.last_column, msg);
}

void errorLogger(char* msg0, char* msg1, char* msg2)
// This is a bad way to handle this, but the other ways inside c are even more crappy
{
	// TODO @Benedikt please fix this
	errors++;
	printf("ERROR: %s%s%s", msg0, msg1, msg2);
	// printf("Following Message is sent:\n(%d.%d-%d.%d): %s\n", yylloc.first_line, yylloc.first_column,
	// yylloc.last_line, yylloc.last_column, msg);
}