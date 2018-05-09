// C program for linked list implementation of stack
#include "stack.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A structure to represent a stack

/**
 * @brief creates a new Stack element
 * 
 * @param data content of the new element
 * @return STACK* returns a stack object
 */
STACK* newNode(char* data)
{
	STACK* stackNode = (STACK*)malloc(sizeof(STACK));
	stackNode->data = (char*)malloc(sizeof(data));
	strcpy(stackNode->data, data);
	stackNode->next = NULL;
	return stackNode;
}

/**
 * @brief checks if stack is empty
 * 
 * @param root stack to check
 * @return int 1 if root is empty
 */
int isEmpty(STACK* root) { return !root; }

/**
 * @brief pushs new object to stack
 * 
 * @param root stack which is used
 * @param data content of the new element
 */
void push(STACK** root, char* data)
{
	isEmpty(*root);
	STACK* stackNode = newNode(data);
	stackNode->next = *root;
	*root = stackNode;
}

/**
 * @brief pop the top element of the stack
 * 
 * @param root stack which is used
 * @param returnval has the value of the top element
 */
void pop(STACK** root, char** returnval)
{
	if (isEmpty(*root))
	{
		(*returnval) = "-1";
		return;
	}
	STACK* temp = *root;
	*root = (*root)->next;
	char* popped = temp->data;
	free(temp);
	(*returnval) = popped;
}

/**
 * @brief returns the value of the top element of the stack
 * 
 * @param root stack to be used
 * @param returnval has the value of the top element
 */
void peek(STACK* root, char** returnval)
{
	if (isEmpty(root))
	{
		(*returnval) = "-1";
		return;
	}
	(*returnval) = root->data;
}