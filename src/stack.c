// C program for linked list implementation of stack
#include "stack.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A structure to represent a stack

STACK* newNode(char* data)
{
	STACK* stackNode = (STACK*)malloc(sizeof(STACK));
	stackNode->data = (char*)malloc(sizeof(data));
	strcpy(stackNode->data, data);
	stackNode->next = NULL;
	return stackNode;
}

int isEmpty(STACK* root) { return !root; }

void push(STACK** root, char* data)
{
	isEmpty(*root);
	STACK* stackNode = newNode(data);
	stackNode->next = *root;
	*root = stackNode;
}

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

void peek(STACK* root, char** returnval)
{
	if (isEmpty(root))
	{
		(*returnval) = "-1";
		return;
	}
	(*returnval) = root->data;
}

int main2()
{
	STACK* root = NULL;
	char* temp;
	push(&root, "10");
	push(&root, "20");
	push(&root, "30");
	pop(&root, &temp);
	printf("%s popped from stack\n", temp);

	// printf("Top element is %s\n", peek(root));

	return 0;
}