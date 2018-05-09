#include "checker.h"
#include "logger.h"
#include "stack.h"

extern STRUCTFUNC* functions;
extern STRUCTVAR* variables;
extern SCOPESTACK* scopes;
extern STRUCTPARAM* parameters;
extern STACK* programstack;

/**
 * \brief Checks if both Parameters are type INT.
 * \param leftvarType Used for the left side of an expression.
 * \param rightVarType Used for the right side of an expression. 
 * \return Indicates that both Parameters are Int.
 *         	1 = leftVarType and rightVarType are INT
 *         	0 = leftVarType and/or rightVarType are/is something else
 */
int isTypeCompatible(char* leftVarType, char* rightVarType)
{
	if (isInt(leftVarType) && isInt(rightVarType))
	{
		return 1;
	}
	return 0;
}

/**
 * \brief Checks if varType is an Integer
 * \param varType TODO
 * \return Indicates if varType is Int.
 *         		1 = varType is INT
 *         		0 = varType is something else
 */
int isInt(char* varType)
{
	if (!strcmp(varType, "INT"))
	{
		return 1;
	}
	return 0;

}
/**
 * \brief Check if the given variable already exists.
 * \param varId 	The id/name of the variable.
 * \param allScope	
 * \return
 * 
 * ToDo Merlin
 */
int varExists(char* varId, int allScopes)
{
	SCOPESTACK* tempscope;
	STRUCTVAR* temp;
	tempscope = scopes;
	for (temp = variables; temp != NULL; temp = temp->hh.next)
	{
		if (!strcmp(temp->id, varId))
		{
			break;
		}
	}
	if (temp != NULL)
		return 1;
	if (!allScopes)
		return 0;
	while (tempscope != NULL)
	{
		for (temp = tempscope->scope; temp != NULL; temp = temp->hh.next)
		{
			if (!strcmp(temp->id, varId))
			{
				break;
			}
		}
		if (temp != NULL)
		{
			return 1;
		}
		tempscope = tempscope->next;
	}
	return 0;
}

/**
 * \brief Checks if the givn function already exists.
 * \param funcId The Name of the function.
 * \return
 * 
 * ToDo Merlin
 */
int funcExists(char* funcId)
{
	STRUCTFUNC* temp;
	for (temp = functions; temp != NULL; temp = temp->hh.next)
	{
		if (!strcmp(temp->id, funcId))
		{
			break;
		}
	}
	if (temp != NULL)
	{
		return 1;
	}
	return 0;
}

/**
 * \brief Checks if the given function is already defined.
 * \param funcId The name of the function.
 * \return
 * 
 * ToDo Merlin
 */
int funcIsDefined(char* funcId)
{
	STRUCTFUNC* temp;
	for (temp = functions; temp != NULL; temp = temp->hh.next)
	{
		if (!strcmp(temp->id, funcId))
		{
			break;
		}
	}
	if (temp->isDefined == 1)
	{
		return 1;
	}
	return 0;
}

/**
 * ToDo Merlin
 * \brief
 * \param funcId		The name of the function.
 * \param type			The return type of the function.
 * \param errorLineInfo The info on the lines and collumns where the error occured.
 */
void checkReturnParam(char* funcId, char* type, ERRORLINEINFO* errorLineInfo)
{
	STRUCTVAR* tempvars;
	tempvars = variables;
	while (tempvars != NULL)
	{
		if (!strcmp(tempvars->id, "functionsReturnParameter") && strcmp(tempvars->type, type))
		{
			errorLogger("Type-Error: Return-Type in Function \"", funcId, "\" has the wrong type!\n", errorLineInfo);
		}
		tempvars = tempvars->hh.next;
	}
}

/**
 * ToDo Merlin
 * \brief
 * \param funcId			The name of the function.
 * \param numberOfParams	The parameter count of the function.
 * \param errorLineInfo		The info on the lines and collumns where the error occured.
 */
void checkFuncCallParams(char* funcId, int numberOfParams, ERRORLINEINFO* errorLineInfo)
{
	STRUCTFUNC* temp;
	STRUCTPARAM* tempParam;
	char* tempParam2;
	for (temp = functions; temp != NULL; temp = temp->hh.next)
	{
		if (!strcmp(temp->id, funcId))
		{
			break;
		}
	}
	if (temp == NULL)
	{
		errorLogger("Unknown function error: Function \"", funcId, "\" doesn't exist!\n", errorLineInfo);
		return;
	}
	if (numberOfParams != temp->paramcount)
	{
		errorLogger("Parameter-Error: Function \"", funcId, "\" is called with the wrong ammount of Parameters!\n",
			errorLineInfo);
		while (numberOfParams > 0)
		{
			pop(&programstack, &tempParam2);
			numberOfParams--;
		}
		return;
	}

	while (numberOfParams > 0)
	{
		tempParam = temp->funcparams;
		for (int i = 1; i < numberOfParams; i++)
		{
			tempParam = tempParam->next;
		}
		pop(&programstack, &tempParam2);
		if (strcmp(tempParam2, tempParam->type))
		{
			errorLogger("Type-Error: Parameter \"", tempParam->name, "\" in Function Call has the wrong Type!\n",
				errorLineInfo);
		}
		numberOfParams--;
	}
	
}

/**
 * ToDo Merlin
 * \brief
 * \param funcId			The name of the function.
 * \param numberOfParams	The parameter count of the function.
 * \param errorLineInfo		The info on the lines and collumns where the error occured.
 */
int checkFuncParams(char* funcId, int numberOfParams, ERRORLINEINFO* errorLineInfo)
{
	STRUCTFUNC* temp;
	STRUCTPARAM *tempParam, *tempParam2;
	for (temp = functions; temp != NULL; temp = temp->hh.next)
	{
		if (!strcmp(temp->id, funcId))
		{
			break;
		}
	}
	if (temp == NULL)
	{
		return 0;
	}

	// Parameter checking from here
	if (numberOfParams == temp->paramcount)
	{
		tempParam = temp->funcparams;
		tempParam2 = parameters;
		while (numberOfParams > 0)
		{
			if (strcmp(tempParam2->type, tempParam->type))
			{
				errorLogger("Type-Error: Parameter \"", tempParam->name,
					"\" in Function Definition has the wrong Type!\n", errorLineInfo);
			}
			numberOfParams--;
			tempParam2 = tempParam2->next;
			tempParam = tempParam->next;
		}
		return 1;
	}
	else
	{
		errorLogger("Parameter-Error: Function \"", funcId, "\" is defined with the wrong ammount of Parameters!\n",
			errorLineInfo);
	}
	return 0;
}


/**
 * \brief Checks if the given function has a valid return type (int or void).
 * \param funcId	The name of the function.
 * \param type		The return type of the function.
 */
int checkFuncType(char* funcId, char* type)
{
	STRUCTFUNC* temp;
	for (temp = functions; temp != NULL; temp = temp->hh.next)
	{
		if (!strcmp(temp->id, funcId))
		{
			break;
		}
	}
	if (!strcmp(temp->type, type))
	{
		return 1;
	}
	return 0;
}

/**
*	Checks if the given variable has the requested type.
*
*	Parameters:
*	- varId :		Name of the variable to be checked
*	- type :		Type the variable should have
*	- allScopes :	"1" = the variable has to be searched in every scope
*					"0" = the variable has to be searched in the current scope only
*	Return:
*	1 :	Variable has the wanted type
*	0 : else
*
*/

/**
 * \brief Checks if the given variable has the requested type.
 * \param varId 	Name of the variable to be checked.
 * \param type		Type the variable should have.
 * \param allScopes	Indicates if the variable has to be searched for in every scope known to mankind.
 * 						"1" = every Scope
 * 						"0" = only the current Scope
 */
int checkVarType(char* varId, char* type, int allScopes)
{
	SCOPESTACK* tempscope;
	STRUCTVAR* temp;
	STRUCTPARAM* tempParam;
	tempscope = scopes;
	for (temp = variables; temp != NULL; temp = temp->hh.next)
	{
		if (!strcmp(temp->id, varId))
		{
			break;
		}
	}
	if (temp != NULL && !strcmp(temp->type, type))
		return 1;
	if (!allScopes)
		return 0;
	while (tempscope != NULL)
	{
		for (temp = tempscope->scope; temp != NULL; temp = temp->hh.next)
		{
			if (!strcmp(temp->id, varId))
			{
				break;
			}
		}
		if (temp != NULL && !strcmp(temp->type, type))
			return 1;
		tempscope = tempscope->next;
	}
	for (tempParam = parameters; tempParam != NULL; tempParam = tempParam->next)
	{
		if (!strcmp(tempParam->name, varId))
		{
			break;
		}
	}
	if (tempParam != NULL)
	{
		return 1;
	}
	return 0;
}