// Author: Caleb Loomis
// Assignment Number: Lab 3
// File Name: CalebLoomisLab3.zip
// Course/Section: CS2123 Section 1
// Date: July 15, 2020
// Instructor: Dr. Ku
#include "CalebLoomisStack.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

//Utility Functions. I'm going to have a CalebLoomisUtilities.h eventually.
int intInput (char * prompt);
char * input (char * prompt, int size);
FILE * promptAndOpenFile ();
char * readLine(FILE * file, int size);
void stripNewline (char * charPtr);

// UI Functions
void welcome(char * c);
int menu(char ** choices, int size);

// Validation and error handling
int isOperator (char c);
int isNumber (char c);
int operate (int op1, int op2, char c);

// options
void processPostfixFromStdin();
void processPostfixFromFile();
int processLine(char * line);

// Infix Functions
void processInfixFromStdin();
void processInfixFromFile();
int processInfixLine(char * line);

int main( ) {

    int lineSize = 64;
    int currentlyRunning = 1;
    char * options [] = {
        "Process Postfix from Stdin",
        "Process Postfix from File",
        "Quit"
        };

    while (currentlyRunning){
        FILE * inputFile;
        char * data;
        welcome ("Postfix / infix calculator");
        int choice = menu (options, 3);
        switch (choice) {
            case 1:
                processPostfixFromStdin();
                break;
            case 2:
                processPostfixFromFile();
                break;
            case 3:
                exit(0);
                break;
            default:
                break;
        }
        currentlyRunning = 0;
    }
    return 0;
}

// UTILITY Functions
// =============================================================================

// +====================== IPO CHART ======================+
// | INPUT:                                                |
// | A string prompt.                                      |
// | PROCESS:                                              |
// | Uses printf and scanf to ask the user the prompt.     |
// | OUTPUT:                                               |
// | User's input, as int.                                 |
// +=======================================================+
int intInput (char * prompt){

    char * step1 = input(prompt, 32);
    int answer = atoi(step1);

	return answer;
}

// +====================== IPO CHART ======================+
// | INPUT:                                                |
// | A string prompt.                                      |
// | PROCESS:                                              |
// | Uses printf and scanf to ask the user the prompt.     |
// | OUTPUT:                                               |
// | User's input, as char *                               |
// +=======================================================+
char * input (char * prompt, int size){
    char * output = malloc ( sizeof (char) * size);
    printf("%s", prompt);
    fgets (output, size, stdin);
    stripNewline(output);

    return output;
}

// +====================== IPO CHART ======================+
// | INPUT:                                                |
// | PROCESS:                                              |
// | Prompts user for filename, verifies its a file and    |
// | opens the file                                        |
// | OUTPUT:                                               |
// | FILE * the file in question                           |
// +=======================================================+
FILE * promptAndOpenFile (){
    FILE * output;
    int accessed = 0;
    char * filename = malloc ( sizeof (char) * 32);

    while (!accessed){
        filename = input("Enter filename\n>>> ", 32);

        if( access( filename, F_OK ) != -1 ) {
            accessed = 1;
            output = fopen(filename, "r");
            if (output == 0){
                printf("Cannot open %s.\n", filename);
                accessed = 0;
            }
        } else {
            printf ("File not found: %s. Please try again.\n", filename);
        }
    }

    return output;
}

// +====================== IPO CHART =========================+
// | INPUT:                                                   |
// | Fptr file, int size                                      |
// | PROCESS:                                                 |
// | returns next string of size size crom file. stripping \n |
// | OUTPUT:                                                  |
// | File pointer                                             |
// +==========================================================+
char * readLine(FILE * file, int size){
    char * output = malloc( sizeof(char) * size);
    fgets (output, size, file);
    stripNewline (output);
    return output;

}

// +====================== IPO CHART =========================+
// | INPUT:                                                   |
// | Char pointer charPtr                                     |
// | PROCESS:                                                 |
// | replaces \n with \0, avoiding newline character.         |
// | OUTPUT:                                                  |
// | File pointer                                             |
// +==========================================================+
void stripNewline (char * charPtr){
    charPtr[strcspn ( charPtr, "\n" )] = '\0';
    if (charPtr == "\n"){
        charPtr = NULL;
    }
}

// +====================== IPO CHART =========================+
// | INPUT:                                                   |
// | Char pointer c                                           |
// | PROCESS:                                                 |
// | Clears screen and welcomes user. Prints c also           |
// +==========================================================+
void welcome(char * c){
    system("clear");
    printf("========== WELCOME ==========\n");
    printf("Welcome to %s\n", c);
}

// +====================== IPO CHART =========================+
// | INPUT:                                                   |
// | Char pointer pointer choices, int size                   |
// | PROCESS:                                                 |
// | Prints size elements from the array of strings, takes    |
// | stdin input and validates its in range.                  |
// +==========================================================+
int menu(char ** choices, int size){
    int i = 0;
    int num;

    printf("Select an option and enter its number below:\n");
    for (i = 0; i < size; i++){
        printf("%i. %s\n", i + 1, choices[i]);
    }

    do {
        num = intInput("\nEnter option number\n>>> ");
        if (num < 1 || num > size){
            printf ("Number out of range, try again.\n");
        }
    } while (num < 1 || num > size);

    return num;
}

// Validation Functions
// ============================================================================

// Input: character c
// Process: Switch case for each valid operator listed on instructions
// Output: 1 if valid. 0 if invalid.
int isOperator (char c) {
    int result = 0;
    switch (c) {
        case '+':
            result = 1;
            break;
        case '-':
            result = 1;
            break;
        case '/':
            result = 1;
            break;
        case '*':
            result = 1;
            break;
        default:
            result = 0;
            break;
    }
    return result;
}

// Input: ints op1 and op2, char c. Assumes char c is a valid operator.
// Process: Switch case for each each valid operator in instructions. Preforms
// These operations.
// Output: result of the oparation between op1 and op2
int operate (int op1, int op2, char c) {
    int result = 0;
    switch (c) {
        case '+':
            result = op1 + op2;
            break;
        case '-':
            result = op1 - op2;
            break;
        case '/':
            result = op1 / op2;
            break;
        case '*':
            result = op1 * op2;
            break;
        default:
            break;
    }
    return result;
}

// Input: character c
// Process: Switch case for each number 0..9
// Output: 1 if valid. 0 if invalid.
int isNumber (char c){
    int result = 0;
    int convertedToNum = c - '0';
    switch (convertedToNum) {
        case 0:
            result = 1;
            break;
        case 1:
            result = 1;
            break;
        case 2:
            result = 1;
            break;
        case 3:
            result = 1;
            break;
        case 4:
            result = 1;
            break;
        case 5:
            result = 1;
            break;
        case 6:
            result = 1;
            break;
        case 7:
            result = 1;
            break;
        case 8:
            result = 1;
            break;
        case 9:
            result = 1;
            break;
        default:
            result = 0;
            break;
    }
    return result;
}

// The actual Assignment Postfix stuff
// ============================================================================

// Input:
// Process: Prompts for user input as a postfix operation. Calls processLine for
// result of postfix. Asks if user would like to run another after.
// Output:
void processPostfixFromStdin(){
    char * toRepeat;
    char * lineToProcess = malloc ( sizeof (char) * 128);
    lineToProcess = input ("Enter Postfix operation (example 2 3 4 + *): ", 128);
    processLine(lineToProcess);
    toRepeat = input("Would you like to enter another line? (y/n) [default no]\n>>>", 8);
    if (toRepeat[0] == 'y'){
        processPostfixFromStdin();
    }
}

// Input:
// Process: Prompts for user input as a filename. Calls processLine for
// each individual line of the file. Asks user if they'd like to continue each time.
// Output:
void processPostfixFromFile(){
    FILE * inFile = promptAndOpenFile();
    char * currentLine;
    char * toRepeat;

    currentLine = readLine(inFile, 128);
    while (currentLine && currentLine[0] != '\0'){
        printf("Current Line is: %s\n", currentLine);
        processLine(currentLine);
        toRepeat = input("Would you like to read the next line? (y/n) [default yes]\n>>>", 8);
            if (toRepeat[0] == 'n'){
                currentLine = NULL;
            } else {
                currentLine = readLine(inFile, 128);
            }
    }
}

// Input: A string called line.
// Process: Follows the postfix algorithm to evaluate postfix. Ignores spaces.
// Output: result of postfix
int processLine(char * line){
    stack myStack = makeNewStack();
    int hasError = 0;
    int i;
    int finalResult = 0;

    for (i = 0; i < strlen(line); i ++){
        if (line [i] == ' '){
            // Do nothing, check nothing more.

        } else if (isNumber(line [i])) { //Push numbers onto the stack
            push(myStack, line[i] - '0');

        } else if (isOperator(line [i])){ //operate when we find operators
            int op1, op2;

            if (!isEmpty(myStack)){
                op2 = pop(myStack);
            }

            if (!isEmpty(myStack)){
                op1 = pop(myStack);
                int result = operate (op1, op2, line[i]);
                push(myStack, result);

            } else { // We only need to error check the second. If its empty it wouldn't work the second time anyways.
                fprintf(stderr, "Error. Malformed Expression.\n");
                hasError = 1;
                i = 1000; // Push i out of the range of the line.
            }

        } else { // If it's neither space, an operator nor a num
            fprintf(stderr, "Error. Invalid character %c found.\n", line[i]);
            hasError = 1;
            i = 1000; // Push i out of the range of the line.
        }
    }
    if (!isEmpty(myStack)){finalResult = pop(myStack);}


    //Finally, make sure the stack is empty.
    if (isEmpty(myStack) && !hasError){
        printf("The value of this expression is: %i\n", finalResult);
    } else if (!isEmpty(myStack) && !hasError){
        printf("ERROR. Malformed Expression\n");
    }

    deleteStack(myStack);
    return finalResult;
}


// Sample output
// ============================================================================

// Sample output For postfix
/*
========== WELCOME ==========
Welcome to Postfix / infix calculator
Select an option and enter its number below:
1. Process Postfix from Stdin
2. Process Postfix from File
3. Convert infix to postfix from Stdin
4. Convert infix to postfix from File
5. Quit

Enter option number
>>> 2
Enter filename
>>> postfix.txt
Current Line is: 345*+67*8+9*+
The value of this expression is: 473
Would you like to read the next line? (y/n) [default yes]
>>>y
Current Line is: 1234*+5/-
The value of this expression is: -1
Would you like to read the next line? (y/n) [default yes]
>>>y
Current Line is: 12+43-*
The value of this expression is: 3
Would you like to read the next line? (y/n) [default yes]
>>>y
Current Line is: 48+65-*32-22+*/     /* haha it uncommented here accidentally
The value of this expression is: 3
Would you like to read the next line? (y/n) [default yes]
>>>y
Current Line is: 234+*
The value of this expression is: 14
Would you like to read the next line? (y/n) [default yes]
>>>y
Current Line is: 72/
The value of this expression is: 3
Would you like to read the next line? (y/n) [default yes]
>>>y
Current Line is: 512+4*+3-
The value of this expression is: 14
Would you like to read the next line? (y/n) [default yes]
>>>y
Current Line is: 72%
Error. Invalid character % found.
Would you like to read the next line? (y/n) [default yes]
>>>y
Current Line is: 7*22+
Error. Malformed Expression.
Would you like to read the next line? (y/n) [default yes]
>>>y
Current Line is: A2/
Error. Invalid character A found.
Would you like to read the next line? (y/n) [default yes]
>>>y
Current Line is: 72//
Error. Malformed Expression.
Would you like to read the next line? (y/n) [default yes]
>>>y
Current Line is: (23-
Error. Invalid character ( found.
Would you like to read the next line? (y/n) [default yes]
>>>y
*/
