#include "checker.h"

int isTypeCompatible(char* leftVarType, char* rightVarType)
{
	if ((!strcmp(leftVarType, "INT")) && (!strcmp("INT", rightVarType)))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int isInt(char* varType)
{
	if (strcmp(varType, "INT"))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}