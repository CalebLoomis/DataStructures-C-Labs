// Author: Caleb Loomis
// Assignment Number: Lab 3
// File Name: CalebLoomisLab3.zip
// Course/Section: CS2123 Section 1
// Date: July 15, 2020
// Instructor: Dr. Ku

#ifndef LoomisStack
#define LoomisStack

union Data;
struct Node;
typedef struct Node *pNode;
typedef pNode stack;

stack makeNewStack();
int isEmpty (stack s);
void clearStack (stack s);
void push (stack s, int i);
int top (stack s);
int pop (stack s);
void err (char * str);
void deleteStack (stack s);

#endif
