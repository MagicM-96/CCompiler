#include "logger.h"
#include <stdio.h>

extern int errors;
extern TEMPCODESTRING* firstTempCode;

extern STRUCTVAR* variables;
extern STRUCTFUNC* functions;

STRUCTVAR* tempVars;
STRUCTFUNC* tempFuncs;

/**
 * \brief Print Symboltable to console.
 *
 * Get the information about variables, functions and parameters and print themto the console inside an ordered
 * Symboltable.
 */
void printSymTable()
{
	printf("\n\n-----Symboltable---------------------------------------------------\n");
	// print variables-subtable
	printf("Global variables:\n");
	if (variables == NULL)
	{
		printf("\tNo global variables were found.\n");
	}
	else
	{
		printf("\tid\t\ttype\t\tvalue\n");
		printf("\t--\t\t----\t\t-----\n");
		for (tempVars = variables; tempVars != NULL; tempVars = tempVars->hh.next)
		{
			printf("\t%s\t\t%s\t\t%d", tempVars->id, tempVars->type, tempVars->value);
			if (tempVars->size > 1)
			{
				printf("\tarray-size: %d", tempVars->size);
			}
			printf("\n");
		}
	}

	// print functions-subtable
	printf("\nFunctions:");
	if (functions == NULL)
	{
		printf("\tNo functions were found.\n");
	}
	else
	{
		for (tempFuncs = functions; tempFuncs != NULL; tempFuncs = tempFuncs->hh.next)
		{
			printf("\n");
			printf("- id: %s, type: %s, paramcount: %d\n", tempFuncs->id, tempFuncs->type, tempFuncs->paramcount);
			// print parameter-subsubtable
			if (tempFuncs->funcparams != NULL)
			{
				STRUCTPARAM* tempParam = tempFuncs->funcparams;
				printf("\tParameters:\n");
				printf("\t|number\t\ttype\t\tname\n");
				printf("\t+------\t\t----\t\t----\n");
				while (tempParam != NULL)
				{
					printf("\t|%d\t\t%s\t\t%s", tempParam->paramNr, tempParam->type, tempParam->name);
					if (tempParam->size > 1)
					{
						printf("\tarray-size: %d", tempParam->size);
					}
					printf("\n");
					tempParam = tempParam->next;
				}
			}
			printf("\n");
			if (tempFuncs->funcvars != NULL)
			{
				STRUCTVAR* tempInnerVars = tempFuncs->funcvars;
				printf("\tInner variables:\n");
				printf("\t|id\t\ttype\t\tvalue\n");
				printf("\t+--\t\t----\t\t-----\n");
				for (; tempInnerVars != NULL; tempInnerVars = tempInnerVars->hh.next)
				{
					printf("\t|%s\t\t%s\t\t%d", tempInnerVars->id, tempInnerVars->type, tempInnerVars->value);
					if (tempInnerVars->size > 1)
					{
						printf("\tarray-size: %d", tempInnerVars->size);
					}
					printf("\n");
				}
			}
		}
	}
	printf("-------------------------------------------------------------------\n");
}

/**
 * @brief Output for all Errors that could occure
 * 
 * @param msg0 Part one of the Message, can be any String in any length
 * @param msg1 Part one of the Message, can be any String in any length
 * @param msg2 Part one of the Message, can be any String in any length
 * @param errorLineInfo The info on the lines and collumns where the error occured.
 */
void errorLogger(char* msg0, char* msg1, char* msg2, ERRORLINEINFO* errorLineInfo)
{
	errors++;
	printf("\n|> ERROR: %s%s%s\n|> START: %d : %d --> END: %d : %d", msg0, msg1, msg2, errorLineInfo->firstLine,
		errorLineInfo->firstColumn, errorLineInfo->lastLine, errorLineInfo->lastColumn);
}

// Functions for most of the Errors that could possibly be thrown.
/**
 * \brief Output for Type-Errors in if statements.
 * \param errorLineInfo The info on the lines and collumns where the error occured.
 */
void throwIfStatementError(ERRORLINEINFO* errorLineInfo)
{
	errorLogger("If-Statement", ":", "Incompatible variable type!", errorLineInfo);
}

/**
 * \brief Output for Type-Errors in while loops.
 * \param errorLineInfo The info on the lines and collumns where the error occured.
 */
void throwWhileLoopError(ERRORLINEINFO* errorLineInfo)
{
	errorLogger("While-loop", ": ", "Incompatible variable type!", errorLineInfo);
}

/**
 * \brief Output for Type-Errors in assignments.
 * \param errorLineInfo The info on the lines and collumns where the error occured.
 */
void throwAssignmentError(ERRORLINEINFO* errorLineInfo)
{
	errorLogger("Assignment", ": ", "Incompatible variable type!", errorLineInfo);
}

/**
 * \brief Output for Type-Errors in logical comparisons.
 * \param errorLineInfo The info on the lines and collumns where the error occured.
 */
void throwLogCompError(ERRORLINEINFO* errorLineInfo)
{
	errorLogger("Logical comparison", ": ", "Incompatible variable type!", errorLineInfo);
}

/**
 * \brief Output for Type-Errors in math operations.
 * \param errorLineInfo The info on the lines and collumns where the error occured.
 */
void throwMathOpError(ERRORLINEINFO* errorLineInfo)
{
	errorLogger("Math Operation", ": ", "Incompatible variable type!", errorLineInfo);
}

/**
 * \brief Output for Type-Errors in shift operations.
 * \param errorLineInfo The info on the lines and collumns where the error occured.
 */
void throwShiftOpError(ERRORLINEINFO* errorLineInfo)
{
	errorLogger("Shift Operation", ": ", "Incompatible variable type!", errorLineInfo);
}

/**
 * \brief Prints the entire Tempcode.
 */
void printStr()
{
	if (firstTempCode == NULL)
	{
		return;
	}
	TEMPCODESTRING* temp = firstTempCode;
	while (temp->line != NULL)
	{
		printf("%s\n", temp->line);
		temp = temp->next;
	}
}
