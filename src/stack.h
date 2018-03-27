#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

typedef struct StackNode
{
    char* data;
    struct StackNode* next;
} STACK;

STACK* newNode(char* data);
int isEmpty(STACK *root);
void push(STACK** root, char* data);
void pop(STACK** root,char** returnval);
void peek(STACK* root,char** returnval);
int main2();