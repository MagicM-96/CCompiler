#include "checker.h"
#include "stack.h"
#include "structs.h"
#include "logger.h"

extern STRUCTFUNC* functions;
extern STRUCTVAR* variables;
extern SCOPESTACK* scopes;
extern STRUCTPARAM* parameters;

int isTypeCompatible(char* leftVarType, char* rightVarType)
{
	if (isInt(leftVarType) && isInt(rightVarType))
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



void checkReturnParam(char* id, char* type)
{
	STRUCTVAR* tempvars;
	tempvars = variables;
	while(tempvars!=NULL)
	{
		if(!strcmp(tempvars->id,"functionsReturnParameter"))
		{
			if(strcmp(tempvars->type,type))
			{
				errorLogger("Type-Error: Return-Type in Function \"",id,"\" has the wrong type!\n");
			}
		}
		tempvars = tempvars->hh.next;
	}
}

int checkFuncParams(char* funcId,int numberOfParams)
{
	STRUCTFUNC* temp;
	STRUCTPARAM* tempParam,*tempParam2;
	for (temp = functions; temp != NULL; temp = temp->hh.next)
	{
		if (!strcmp(temp->id, funcId))
		{
			break;
		}
	}
	if (temp == NULL)
		return 0;
	
	//Parameter checking from here
	if(numberOfParams==temp->paramcount)
	{
		tempParam = temp->funcparams;
		tempParam2 = parameters;
		while(numberOfParams>0)
		{
			if(strcmp(tempParam2->type,tempParam->type))
			{
				errorLogger("Type-Error: Parameter Nr. \"",tempParam->name,"\" in Function Call has the wrong Type!\n");
			}
			numberOfParams--;
			tempParam2 = tempParam2->next;
			tempParam = tempParam->next;
		}
	}
	else
	{
		errorLogger("Parameter-Error: Function \"",funcId,"\" is called with the wrong ammount of Parameters!\n");
	}
	return 0;
}

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
	if(!allScopes)
		return 0;
	while(tempscope!=NULL)
	{
		for (temp = tempscope->scope; temp != NULL; temp = temp->hh.next)
		{
			if (!strcmp(temp->id, varId))
			{
				break;
			}
		}
		if (temp != NULL)
			return 1;
		tempscope = tempscope->next;
	}
	return 0;
}

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
		return 1;
	return 0;
}

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
		return 1;
	return 0;
}

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
	if (!strcmp(temp->type,type))
		return 1;
	return 0;
}


/*
	Checks if the given variable has the requested type.

	Parameters:
	- varId :		Name of the variable to be checked
	- type :		Type the variable should have
	- allScopes :	"1" = the variable has to be searched in every scope
					"0" = the variable has to be searched in the current scope only
	Return:
	1 :	Variable has the wanted type
	0 : else

*/
int checkVarType(char* varId,char* type,int allScopes)
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
	if (temp != NULL&& !strcmp(temp->type,type))
		return 1;
	if(!allScopes)
		return 0;
	while(tempscope!=NULL)
	{
		for (temp = tempscope->scope; temp != NULL; temp = temp->hh.next)
		{
			if (!strcmp(temp->id, varId))
			{
				break;
			}
		}
		if (temp != NULL&& !strcmp(temp->type,type))
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
