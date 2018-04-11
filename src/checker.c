#include "checker.h"

int isTypeCompatible(char* leftVarType, char* rightVarType)
{
	if (!strcmp(leftVarType, "INT") && !strcmp("INT", rightVarType))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * \brief Checks if varType is an Integer
 * \param varType TODO
 * \return Indicates if varType is Int.
 *         1 = varType is INT
 *         0 = varType is something else
 */
int isInt(char* varType)
{
	if (!strcmp(varType, "INT"))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
