#include "symboltable.h"

#include "checker.h"
#include "logger.h"
#include "stack.h"
#include "tempcode.h"
#include <string.h>

STACK* programstack;
STRUCTFUNC* functions = NULL;
STRUCTVAR* variables = NULL;
SCOPESTACK* scopes = NULL;
STRUCTPARAM* parameters = NULL;

void addFunc(char* id, char* type, int numberOfParams, ERRORLINEINFO* errorLineInfo)
{
	if (!funcExists(id) && !varExists(id, 1))
	{
		STRUCTFUNC* s;
		s = (STRUCTFUNC*)malloc(sizeof(STRUCTFUNC));
		s->id = (char*)malloc(sizeof(id));
		s->type = (char*)malloc(sizeof(type));
		strcpy(s->id, id);
		strcpy(s->type, type);
		s->paramcount = numberOfParams;
		s->isDefined = 0;
		STRUCTPARAM* p = NULL;
		STRUCTPARAM* tempstruct;
		char* temptype;
		char* tempid;
		char* templength;
		while (numberOfParams > 0)
		{
			pop(&programstack, &templength);
			pop(&programstack, &temptype);
			pop(&programstack, &tempid);
			if (!strcmp(temptype, "VOID"))
			{
				errorLogger("Type-Error: Parameter \"", tempid, "\" can't be declared as VOID!\n", errorLineInfo);
			}
			else
			{
				if (atoi(templength) > 1)
				{
					strcat(temptype, "-ARR");
				}
				tempstruct = p;
				p = (STRUCTPARAM*)malloc(sizeof(STRUCTPARAM));
				p->type = (char*)malloc(sizeof(temptype));
				p->name = (char*)malloc(sizeof(tempid));
				strcpy(p->type, temptype);
				strcpy(p->name, tempid);
				p->paramNr = numberOfParams;
				p->size = atoi(templength);
				p->next = tempstruct;
				numberOfParams--;
			}
		}
		parameters = p;
		s->funcparams = p;
		HASH_ADD_INT(functions, id, s);
		// log_funcs();
	}
	else
	{
		errorLogger("Name-Error: Identifier \"", id, "\" is already in use!\n", errorLineInfo);
	}
}

void addVar(char* id, char* type, int value, int size)
{
	STRUCTVAR* s;
	s = (STRUCTVAR*)malloc(sizeof(STRUCTVAR));
	s->id = (char*)malloc(sizeof(id));
	s->type = (char*)malloc(sizeof(type));
	strcpy(s->id, id);
	strcpy(s->type, type);
	s->value = value;
	s->size = size;
	HASH_ADD_INT(variables, id, s);
	char* temp = (char*)malloc(sizeof(char)*3);
	sprintf(temp,"%d",size);
	if(strcmp(type,"ARRAY-ELEMENT")&&strcmp(id,"functionsReturnParameter"))
		addCode(DECVAR,NULL,type,id,temp);
}

void defineFunc(char* id, char* type, int numberOfParams, ERRORLINEINFO* errorLineInfo, char* label)
{
	if (funcExists(id))
	{
		if (checkFuncType(id, type))
		{
			if (funcIsDefined(id))
			{
				errorLogger("Multiple function-Definition: Function \"", id, "\" is already defined!\n", errorLineInfo);
			}
			checkFuncParams(id, numberOfParams, errorLineInfo);
			addVariablesToFunction(id, label);
		}
		else
		{
			errorLogger("Type-Error: Function \"", id, "\" is defined in different Types!", errorLineInfo);
		}
	}
	else if (!varExists(id, 1))
	{
		addFunc(id, type, numberOfParams, errorLineInfo);
		STRUCTFUNC* tempFunc = functions;
		while (strcmp(tempFunc->id, id))
		{
			tempFunc = tempFunc->hh.next;
		}
		tempFunc->funcvars = variables;
		tempFunc->isDefined = 1;
		tempFunc->label = (char*)malloc(sizeof(label));
		strcpy(tempFunc->label, label);
	}
	else
	{
		errorLogger("Name-Error: Identifier \"", id, "\" is already in use!\n", errorLineInfo);
	}
	checkReturnParam(id, type, errorLineInfo);
}

void addVariablesToFunction(char* id, char* label)
{
	STRUCTFUNC* temp = functions;
	while (strcmp(temp->id, id))
	{
		temp = temp->hh.next;
	}
	temp->isDefined = 1;
	temp->funcvars = variables;
	temp->label = (char*)malloc(sizeof(label));
	strcpy(temp->label, label);
}

void startScope()
{
	// push variables on scopestack
	SCOPESTACK* temp;
	temp = (SCOPESTACK*)malloc(sizeof(SCOPESTACK));
	temp->scope = variables;
	temp->next = scopes;
	scopes = temp;
	variables = NULL;
}

void endScope()
{
	if (scopes != NULL)
	{
		variables = scopes->scope;
		SCOPESTACK* temp;
		temp = scopes->next;
		scopes = temp;
	}
	parameters = NULL;
}

void identifierDeclaration(int length, char* type, ERRORLINEINFO* errorLineInfo)
{
	if (!strcmp(type, "0"))
	{
		char* tempid;
		char* temptype;
		pop(&programstack, &tempid);
		if (!strcmp(tempid, "functionsReturnParameter"))
		{
			errorLogger("Name-Error: Variable can't be called \"", tempid,
				"\" because this name is reserved for the compiler!\n", errorLineInfo);
		}
		peek(programstack, &temptype);
		if (!strcmp(temptype, "VOID"))
		{
			return;
		}
		if (varExists(tempid, 0) || funcExists(tempid))
		{
			errorLogger("Name-Error: Identifier \"", tempid, "\" already exists!", errorLineInfo);
			return;
		}
		if (length == 1)
		{
			addVar(tempid, "INT", 0, length);
			return;
		}
		addVar(tempid, "INT-ARR", 0, length);
		char* tempId;
		char* tempI;
		tempId = (char*)malloc(sizeof(tempid) + 2 * sizeof(char) + sizeof(int));
		tempI = (char*)malloc(sizeof(int));
		for (int i = 0; i < length; i++)
		{
			strcpy(tempId, tempid);
			strcat(tempId, "[");
			sprintf(tempI, "%d", i);
			strcat(tempId, tempI);
			strcat(tempId, "]");
			addVar(tempId, "ARRAY-ELEMENT", 0, 1);
		}
		return;
	}

	if (!strcmp(type, "VOID"))
	{
		char* id;
		pop(&programstack, &id);
		errorLogger("Type-Error: Can't declare variable \"", id, "\" as VOID!\n", errorLineInfo);
		return;
	}
	char* id;
	pop(&programstack, &id);
	if (!strcmp(id, "functionsReturnParameter"))
	{
		errorLogger("Name-Error: Variable can't be called \"", id,
			"\" because this name is reserved for the compiler!\n", errorLineInfo);
	}
	push(&programstack, type);

	if (varExists(id, 0) || funcExists(id))
	{
		errorLogger("Name-Error: Identifier \"", id, "\" is already in use!\n", errorLineInfo);
		return;
	}

	if (length == 1)
	{
		addVar(id, "INT", 0, length);
		return;
	}

	addVar(id, "INT-ARR", 0, length);
	char* tempId;
	char* tempI;
	tempId = (char*)malloc(sizeof(id) + 2 * sizeof(char) + sizeof(int));
	tempI = (char*)malloc(sizeof(int));
	for (int i = 0; i < length; i++)
	{
		strcpy(tempId, id);
		strcat(tempId, "[");
		sprintf(tempI, "%d", i);
		strcat(tempId, tempI);
		strcat(tempId, "]");
		addVar(tempId, "ARRAY-ELEMENT", 0, 1);
	}
}

void pushSomething()
{
	push(&programstack, "test1");
	char* temp;

	peek(programstack, &temp);
	pop(&programstack, &temp);
	printf("Stack is: %s\n", temp);
	peek(programstack, &temp);
}

void lookupFunctionType(char* funcId, char** ret)
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
		(*ret) = temp->type;
}
}

void lookupFunctionLabel(char* funcId, char** ret)
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
		(*ret) = temp->label;
	}
}

void lookupVariableType(char* varId, char** ret)
{
	SCOPESTACK* tempscope;
	STRUCTPARAM* tempParam;
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
	{
		(*ret) = temp->type;
		return;
	}
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
			(*ret) = temp->type;
			return;
		}
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
		(*ret) = tempParam->type;
		return;
	}
}
