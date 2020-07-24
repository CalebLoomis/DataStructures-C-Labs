// Author: Caleb Loomis
// Assignment Number: Lab 3
// File Name: CalebLoomisLab3.zip
// Course/Section: CS2123 Section 1
// Date: July 15, 2020
// Instructor: Dr. Ku
#include "CalebLoomisStack.h"
#include <stdlib.h>
#include <stdio.h>

// This program is a linked list stack that takes ints only
// I know I could've used provided code, I just wanted to
// I was going to use a union to store any data type, but ran out of time
// Please note that this was heavily inspired by the book, as I was originally
// Planning to remix the code to support multiple data types.
struct Node{
    int data;
    pNode next;
};

stack makeNewStack (){
    stack s;
    s = malloc( sizeof (struct Node));
    s->next = NULL;
}

// Input: stack
// Output: if the next and data are both null. We need to check the whole union here
int isEmpty (stack s){
    return (s->next == NULL);
}

// Input: stack
// Process: Continual popping until stack is empty.
void clearStack (stack s){
    while (!isEmpty(s)){
        pop(s);
    }
}

// Input: stack
// Input: int i to be added
// Process: Adds i to stack
void push(stack s, int i) {
    pNode tempNode;
    tempNode = malloc( sizeof( struct Node ) );
    tempNode->data = i;
    tempNode->next = s->next;
    s->next = tempNode;
}

// Input: stack s
// Output: returns s->data
int top(stack s){
    if (s->next->data){
        return s->next->data;
    } else {
        err ("Cannot top empty stack!");
        return 0;
    }
}

int pop(stack s){
    int output;
    if (!isEmpty(s)){
        output = top(s);
        pNode tmp = s->next;
        s->next = s->next->next;
        free (tmp);
    } else {
        err ("Cannot pop empty stack!");
    }

    return output;
}

// Input: error str
// Process: prints str to stderr.
void err (char * str){
    fprintf( stderr, "%s\n", str );
}

// input: stack s
// process: pops everything then frees the stack
void deleteStack(stack s){
    clearStack(s);
    free(s);
}
