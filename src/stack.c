// C program for linked list implementation of stack
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
 
// A structure to represent a stack
typedef struct StackNode
{
    char* data;
    struct StackNode* next;
} STACK;

STACK* newNode(char* data);
int isEmpty(STACK *root);
void push(STACK** root, char* data);
char* pop(STACK** root);
const char* peek(STACK* root);
int main2();
 
STACK* newNode(char* data)
{
    STACK* stackNode =
              (STACK*) malloc(sizeof(STACK));
    stackNode->data = data;
    stackNode->next = NULL;
    return stackNode;
}
 
int isEmpty(STACK *root)
{
    return !root;
}
 
void push(STACK** root, char* data)
{
    STACK* stackNode = newNode(data);
    stackNode->next = *root;
    *root = stackNode;
    printf("%s pushed to stack\n", data);
}
 
char* pop(STACK** root)
{
    if (isEmpty(*root))
        return "-1";
    STACK* temp = *root;
    *root = (*root)->next;
    char* popped = temp->data;
    free(temp);
 
    return popped;
}
 
const char* getString2(){
    printf("gets called\n");
  return "Hallo!";
}

const char* peek(STACK* root)
{
    if (isEmpty(root))
        return "-1";
    printf("returns following: %s\n",root->data);
    return "Hallo!";//*(root->data);
}
 
int main2()
{
    STACK* root = NULL;
 
    push(&root, "10");
    push(&root, "20");
    push(&root, "30");
 
    printf("%s popped from stack\n", pop(&root));
 
   // printf("Top element is %s\n", peek(root));
 
    return 0;
}