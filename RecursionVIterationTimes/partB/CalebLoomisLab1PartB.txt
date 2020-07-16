// Author: Caleb Loomis
// Assignment Number: Lab 1 Part B
// File Name: CalebLoomisLab1.zip
// Course/Section: CS2123 Section 1
// Date: June 10, 2020 (Began), June 17, 2020 (Finished)
// Instructor: Dr. Ku

// This program takes a text file, bible.txt, copies or trims or extends bible.txt to
// match a user-defined number of lines as data.txt, then reverses the file.


//COMMON ACRONYMS USED: STR = string
// tmp, temp = Temporary

#include <stdio.h>  // printf, fprintf, scanf, etc.
#include <string.h> // strcpy
#include <stdlib.h> // malloc, realloc
#include <time.h> // clock

// iterative Functions.
void iterativeReverseFile(char * fileName, char * outFileName);
void iterativeReverseLine (char ** line);
void iterativeReverseArray (char *** strArray, int numberOfLines);
void iterativeWriteFile(char ** stringsToWrite, FILE * outFile, int numberOfLines);

//Recrsive Functions
void recursiveReverseFile(char * fileName, char * outFileName);
void recursiveReverseFileTail(FILE * file, FILE * outFile, char *** reversedFile, char * buffer, int position, int multiplyer);
void recursiveReverseLine (char * line, int current);
void recursiveReverseArray (char ** arrayToReverse, int current, int endpoint);
void recursiveWriteArrayToFile (char ** arrayToWrite, int current, int endpoint, FILE * outFile);

//Misc. Functions
int getMidpoint(char * stringToFind);
int getEndpoint(char * stringToFindEndpoint);
void swapChars (char * char1, char * char2);
void swapStrings (char ** string1, char ** string2);
FILE * openFileToRead (char * filename);
FILE * openFileToWrite (char * filename);
int intInput (char * prompt);
FILE * fileAdjustedToLines (FILE * sourceFile, int numberOfLines);

//Timer Functions
double timeIterative(char * inFileName, char * outFileName, int numberOfRepititions);
double timeRecursive(char * inFileName, char * outFileName, int numberOfRepititions);


// +=========================================================================+
// |           Main function.                                                |
// +=========================================================================+
int main() {
    FILE * sourceFile = openFileToRead("bible.txt");
    char * outFileName = "reverse_bible.txt";
    char * inFileName = "data.txt";
    int numberOfLines = intInput("Enter a number of lines for our input file (default 30383):\n>>> ");

    // Make sure the number makes sense
    if (numberOfLines <= 0){
        numberOfLines = 30383; //Default bible.txt size.
    }

    // We know how many lines we want and that it is greater than 0.
    // We will create a new file of that many lines using bible.txt.
    fileAdjustedToLines (sourceFile, numberOfLines);
    fclose(sourceFile);

    int numberOfRepititions = intInput("Number of runs:\n>>> ");

    printf("Processing...\n");
    double iterative = timeIterative(inFileName, outFileName, numberOfRepititions);
    double recursive = timeRecursive(inFileName, outFileName, numberOfRepititions);
    printf(" +==============================+\n");
    printf(" | Iterative version: %.2f      |\n", iterative);
    printf(" | Recursive version: %.2f      |\n", recursive);
    printf(" +==============================+\n\n");

    if (iterative > recursive){
        printf("Recursion is faster by %.2f seconds \n", (iterative - recursive));
    } else {
        printf("Iteration is faster by %.2f seconds.\n", (recursive - iterative));
    }

    return 0;
}



// +===========================================================================+
// |                          ITERATIVE FUNCTIONS START HERE                   |
// +===========================================================================+

// +====================== IPO CHART ======================+
// | INPUT:                                                |
// | File to reverse, output file                          |
// | PROCESS:                                              |
// | First, make sure we have enough room in our array     |
// | and realloc as needed. Each element in our array is a |
// | line of text. While we can still read lines of text,  |
// | reverse each line individually, and store it in an    |
// | array of strings. When we can no longer read lines,   |
// | reverse all elements of our array, then write to the  |
// | outFile                                               |
// | OUTPUT:                                               |
// | Reversed file written to the outFile                  |
// +=======================================================+
void iterativeReverseFile(char * inFileName, char * outFileName){
    //Open our files.
    FILE * file = openFileToRead(inFileName);
    FILE * outFile = openFileToWrite(outFileName);
    // Buffer will be the size of each
    char buffer [600]; //Longest verse in the bible, Esther 8:9 is a bit over 500 characters long.
    // If a line is over 600, we will only do the first 600 chars.
    // I would use a global variable but that's not allowed.

    // We'll allocate 100 character pointers (lines) at a time.
    char ** reversedFile = (char **)malloc(100 * sizeof(char *));
    // This is our counter
    int currentLineNum = 0;

    // This is the multiplyer for our array. We're doing 100 lines at a time.
    // We'll later check if we're at 100 * multiplyer for reallocation.
    int multiplyer = 1;

    while (fgets(buffer, 600, file)){

        // Prevent overflow of the reversedFile array
        if (currentLineNum == (100 * multiplyer)){
            multiplyer ++;
            char ** tmp = realloc(reversedFile, (100 * multiplyer) * sizeof(char *));
            if (tmp == NULL){
                printf("Could not Realloc memory. Quitting!\n" );
            } else {
                reversedFile = tmp;
                tmp = NULL;
            }
        }
        // Get Len of string in buffer
        int bufferLen = strlen(buffer);

        // Let's chop off the newline character
        buffer[bufferLen - 1] = '\0';

        bufferLen --; // We've just chopped off one character.
        // Allocate space for our string that takes up exactly as much memory as we need
        // bufferlen + 1 for enough space for \0.
        char * tempString = (char *) malloc((bufferLen + 1) * sizeof(char));

        // Make sure that it isn't an empty line.
        if (bufferLen != 0){

            // Copy this string from or buffer to the tempString
            strcpy(tempString, buffer);

            // Call iterativeReverseLine
            iterativeReverseLine(&tempString);
            // Assign it to our data structure.
            reversedFile[currentLineNum] = tempString;
            currentLineNum ++;
        }

    }

    // Now call the iterativeReverseArray
    iterativeReverseArray(&reversedFile, currentLineNum);

    // Finally, write the data structure to our outFile
    iterativeWriteFile(reversedFile, outFile, currentLineNum);
    //Close our files
    fclose(outFile);
    fclose(file);
}

// +====================== IPO CHART ======================+
// | INPUT:                                                |
// | pointer to string to reverse                          |
// | PROCESS:                                              |
// | Find our midpoint and endpoint and look at each item  |
// | one at a time. While we are not half through, swap the|
// | current element with element (len - current)          |
// | OUTPUT:                                               |
// | Values of string elements swapped                     |
// +=======================================================+
void iterativeReverseLine (char ** line) {
    //Because remembering *line is annoying
    char * stringToReverse = *line;
    //Get endpoint
    int endpoint = strlen(stringToReverse) - 1;

    //Get midpoint
    int midpoint = getMidpoint(stringToReverse);
    int currentPos = 0;
    for (currentPos = 0; currentPos < midpoint; currentPos ++){
        swapChars(stringToReverse + currentPos, stringToReverse + endpoint);
        endpoint --;
    }
}

// +====================== IPO CHART ======================+
// | INPUT:                                                |
// | pointer to string array to reverse, # of elements     |
// | PROCESS:                                              |
// | Find our midpoint and endpoint and look at each item  |
// | one at a time. While we are not half through, swap the|
// | current element with element (len - current)          |
// | OUTPUT:                                               |
// | Values of array elements swapped                      |
// +=======================================================+
void iterativeReverseArray (char *** strArray, int numberOfLines) {
    //Because remembering * is annoying
    char ** stringArrayToReverse = *strArray;
    //Because numberOfLines is too long and doesn't make sense in algorithm.
    int endpoint = numberOfLines - 1;
    //Yay int division. It'll just let me ignore the "middle" number if there's an odd num.
    int midpoint = endpoint / 2;
    int currentPos;
    for (currentPos = 0; currentPos < midpoint; currentPos ++){
        // Swap the current value with the last point we care about.
        swapStrings ( &stringArrayToReverse[currentPos], &stringArrayToReverse[endpoint]);
        // Because, that char is switched, move back one.
        endpoint --;
    }
}

// +====================== IPO CHART ======================+
// | INPUT:                                                |
// | String array to write, file to write to, number of    |
// | lines to write.                                       |
// | PROCESS:                                              |
// | loop through the array and write each element to file |
// | OUTPUT:                                               |
// | Array written to our outFile.                         |
// +=======================================================+
void iterativeWriteFile(char ** stringsToWrite, FILE * outFile, int numberOfLines){
    int i;
    for (i = 0; i < numberOfLines; i ++){
        fprintf(outFile, "%s\n", stringsToWrite[i]);
    }
}

// +===========================================================================+
// |                          RECURSIVE FUNCTIONS START HERE                   |
// +===========================================================================+


// +====================== IPO CHART ======================+
// | INPUT:                                                |
// | File to reverse, output file                          |
// | PROCESS:                                              |
// | We want to read our first line and get everything set |
// | up for our tail recursion function. Just check that   |
// | the file isn't empty and set up some variables        |
// | OUTPUT:                                               |
// | variables and valid file sent to next function.       |
// +=======================================================+
void recursiveReverseFile(char * inFileName, char * outFileName){
    // Open our file and outFile
    FILE * file = openFileToRead(inFileName);
    FILE * outFile = openFileToWrite(outFileName);
    //Create a buffer string that is 600 chars long.
    //I would use a global variable but that's not allowed.
    char buffer [600];
    int position = 0; //Position in our data structure
    //We will use this multiplyer just like how we used it in our iterative function aboce
    int multiplyer = 1;
    // Here we make the data structure
    char ** reversedFile = (char **)malloc(100 * sizeof(char *));
    //Get the crrent buffer.
    fgets(buffer, 600, file);

    if (buffer == NULL){ //Base Case. Eventually, the file will end or theres an error.
        // This is strange because we didn't get to our tail recursive function.
        printf("End of file before reaching tail recursion.");
        //Close our files and exit
        fclose(file);
        fclose(outFile);
        exit(1);
    } else {
        recursiveReverseFileTail(file, outFile, &reversedFile, buffer, position, multiplyer);
    }
    // After the file is reversed, we can close our files.
    fclose(file);
    fclose(outFile);
}

// +====================== IPO CHART ======================+
// | INPUT:                                                |
// | File to reverse, output file, data structure, buffer  |
// | containing  our next line, the current number of      |
// | elements processed and a multiplyer for the size of   |
// | our data structure, which is just an array of strings |
// | that we realloc every 100 elements                    |
// | PROCESS:                                              |
// | First, make sure we have enough room in our array     |
// | and realloc as needed. Each element in our array is a |
// | line of text. Strip the \n off of the end of buffer.  |
// | reverse our buffer and copy it to a temp string thats |
// | added to our ds. Load the next line. If it's not null,|
// | recursively call this function, Otherwise flip our arr|
// | and write to file.                                    |
// | OUTPUT:                                               |
// | Reversed file written to the outFile                  |
// +=======================================================+
void recursiveReverseFileTail(FILE * file, FILE * outFile, char *** reversedFile,
    char * buffer, int position, int multiplyer){
    //We will check if we still have room in our reversedFile.
    // Prevent overflow of the reversedFile array
    if (position == (100 * multiplyer)){
        multiplyer ++;
        char ** tmp = realloc( * reversedFile, (100 * multiplyer) * sizeof(char *));
        if (tmp == NULL){ //Something went wrong. It happened once but I can't recreate it
        // After I made this catch.
            printf ("Could not Realloc memory. Quitting!\n" );
            exit (1);
        } else {
            * reversedFile = tmp;
            tmp = NULL;
        }
    }

    // A pointer to our DS
    char ** dataStructure = * reversedFile;
    // Get Len of string in buffer
    int bufferLen = strlen(buffer);

    // Let's chop off the newline character
    buffer[bufferLen - 1] = '\0';

    bufferLen --; // We've just chopped off one character.

    //A temp string to copy our buffer into.
    char * tempString = (char *) malloc((bufferLen + 1) * sizeof(char));

    if (bufferLen != 0) { //In case of empty lines. We don't need those.

            // Call recursiveReverseLine to reverse the buffer.
            recursiveReverseLine(buffer, 0);
            strcpy(tempString, buffer);
            dataStructure[position] = tempString;

    }
    // Get the next line before deciding if we are at the base case


    if (fgets(buffer, 600, file)){ // If the buffer isn't empty, we will go to the next line recursively.
        recursiveReverseFileTail(file, outFile, reversedFile, buffer, position + 1, multiplyer);
    } else { //Otherwise, finish flipping the file and write to our reverse file.
        // The else is the base case.
        recursiveReverseArray(dataStructure, 0, position - 1);
        //After we flip the order of the array, then we write to file, recursively.
        recursiveWriteArrayToFile(dataStructure, 0, position, outFile);
        return;
    }
}

// +====================== IPO CHART ======================+
// | INPUT:                                                |
// | String to reverse, current position in string.        |
// | PROCESS:                                              |
// | Find our midpoint. If we are not at least halfway     |
// | through the string, swap our current element with the |
// | element (len - current), then call with current + 1   |
// | OUTPUT:                                               |
// | Values of string elements swapped                     |
// +=======================================================+
void recursiveReverseLine (char * line, int current) {
    int midpoint = getMidpoint(line);
    int endpoint = getEndpoint(line);

    if (current >= midpoint) return;
    //printf("%d\n", current); I had an error before because I put the < when I needed >
    swapChars(&line[current], &line[endpoint - current]);
    current ++;

    recursiveReverseLine(line, current);

}

// +====================== IPO CHART ======================+
// | INPUT:                                                |
// | String array to reverse, current line, # of elements  |
// | PROCESS:                                              |
// | Find our midpoint. If we are not at least halfway     |
// | through the array, swap our current element with the  |
// | element (len - current), then call with current + 1   |
// | OUTPUT:                                               |
// | Values of array elements swapped                      |
// +=======================================================+
void recursiveReverseArray (char ** arrayToReverse, int current, int endpoint) {
    int midpoint = endpoint / 2;
    if (current >= midpoint) return;
    //printf("%d\n", current); I had an error before because I put the < when I needed >
    swapStrings(&arrayToReverse[current], &arrayToReverse[endpoint - current]);
    current ++;

    recursiveReverseArray(arrayToReverse, current, endpoint);
}

// +====================== IPO CHART ======================+
// | INPUT:                                                |
// | String array to write, current line, # of elements in |
// | array, file to write to.                              |
// | PROCESS:                                              |
// | if we are not at the end of the string, write our     |
// | current line, then call this function with current + 1|
// | OUTPUT:                                               |
// | Array written to our outFile.                         |
// +=======================================================+
void recursiveWriteArrayToFile (char ** arrayToWrite, int current, int endpoint, FILE * outFile) {
    if (current >= endpoint) return; //Base case. End of array.
    //printf("%s\n", arrayToWrite[current]);
    fprintf(outFile, "%s\n", arrayToWrite[current]);
    // We want to iterate AFTER we write to file.
    // I feel dumb that this took me 20 minutes to catch.
    current ++;
    recursiveWriteArrayToFile(arrayToWrite, current, endpoint, outFile);

}
// +===========================================================================+
// |                          MISC FUNCTIONS HERE                              |
// +===========================================================================+

// +====================== IPO CHART ======================+
// | INPUT:                                                |
// | a string                                              |
// | PROCESS:                                              |
// | Get the endpoint, then divide by 2                    |
// | OUTPUT:                                               |
// | Midpoint of the string, rounded down.                 |
// +=======================================================+
int getMidpoint(char * stringToFindMidpoint){
    return getEndpoint(stringToFindMidpoint) / 2;
}

// +====================== IPO CHART ======================+
// | INPUT:                                                |
// | a string                                              |
// | PROCESS:                                              |
// | alias for strlen(string) - 1                          |
// | OUTPUT:                                               |
// | strlen(string) - 1                                    |
// +=======================================================+
int getEndpoint(char * stringToFindEndpoint){
    return strlen(stringToFindEndpoint) - 1;
}

// +====================== IPO CHART ======================+
// | INPUT:                                                |
// | two char pointers                                     |
// | PROCESS:                                              |
// | Uses a temp value to swap the values                  |
// | OUTPUT:                                               |
// | Returns nothing, but swaps values                     |
// +=======================================================+
void swapChars (char * char1, char * char2){
    char tmp = * char1;
    * char1 = * char2;
    * char2 = tmp;

    return;
}

// +====================== IPO CHART ======================+
// | INPUT:                                                |
// | two string pointers                                   |
// | PROCESS:                                              |
// | Uses a temp value to swap the values                  |
// | OUTPUT:                                               |
// | Returns nothing, but swaps values                     |
// +=======================================================+
void swapStrings (char ** string1, char ** string2){
    char * tmp = * string1;
    * string1 = * string2;
    * string2 = tmp;

    return;
}

// +====================== IPO CHART ======================+
// | INPUT:                                                |
// | A string filename                                     |
// | PROCESS:                                              |
// | Uses fopen to open file in read mode                  |
// | OUTPUT:                                               |
// | File pointer                                          |
// +=======================================================+
FILE * openFileToRead(char * filename) {
    // Let's open a file
    FILE * file = fopen(filename, "r");
    // Then check if it is actualy open.
    if (file == NULL){
        fprintf(stderr, "ERROR: Could not open file: %s\n", filename);
        exit (1); // This will show that we weren't able to open the file.
    }
}

// +====================== IPO CHART ======================+
// | INPUT:                                                |
// | A string filename                                     |
// | PROCESS:                                              |
// | Uses fopen to open file in write mode                 |
// | OUTPUT:                                               |
// | File pointer                                          |
// +=======================================================+
FILE * openFileToWrite(char * filename){
    return fopen(filename, "w+");
}

// +====================== IPO CHART ======================+
// | INPUT:                                                |
// | A string prompt.                                      |
// | PROCESS:                                              |
// | Uses printf and scanf to ask the user the prompt.     |
// | OUTPUT:                                               |
// | User's input, as int.                                 |
// +=======================================================+
int intInput (char * prompt){
    int answer;
    printf("%s", prompt);
    scanf("%d", &answer);

    return answer;
}

// +====================== IPO CHART ======================+
// | INPUT:                                                |
// | A source file and a numberOfLines                     |
// | PROCESS:                                              |
// | Loops throught n times, writing the file one line at a|
// | time. Resets to top of source file if needed          |
// | OUTPUT:                                               |
// | A new file of specified length                        |
// +=======================================================+
FILE * fileAdjustedToLines (FILE * sourceFile, int numberOfLines){
    FILE * result = fopen ("data.txt", "w+");

    // If, for some reason, 0 is passed here.
    if (numberOfLines <= 0) return result;

    // We're limiting the line size to 600.
    // I would use a global variable but that's not allowed.
    char data [600];

    if (!fgets(data, 600, sourceFile)) {
        fprintf(stderr, "Error reading file.\n");
        return result;
    }

    int x;
    for (x = 0; x < numberOfLines; x ++){
        if (fgets(data, 600, sourceFile)) {
            fprintf(result, "%s", data);
        } else {
            rewind(sourceFile);
        }
    }
    fprintf(result, "\n");
    fclose(result);
    // Output needs to be in read mode.
    return fopen("data.txt", "r");
}

// +===========================================================================+
// |                          TIMER FUNCTIONS START HERE                       |
// +===========================================================================+

// +====================== IPO CHART ======================+
// | INPUT:                                                |
// | An input file, an output file, and the number or reps |
// | PROCESS:                                              |
// | call the iterative reverse function, number times     |
// | OUTPUT:                                               |
// | The amount of time it took to complete                |
// +=======================================================+
double timeIterative(char * inFileName, char * outFileName, int numberOfRepititions){
    // Function only works with positive nums.
    if (numberOfRepititions <= 0) return 0.0;

    // If they did Something normal...
    clock_t timer;
    timer = clock();

    int x;
    for (x = 0; x < numberOfRepititions; x ++){
        iterativeReverseFile(inFileName, outFileName);
    }
    timer = clock() - timer;

    double timeAllotted = ((double) timer) / CLOCKS_PER_SEC;
    return timeAllotted;
}

// +====================== IPO CHART ======================+
// | INPUT:                                                |
// | An input file, an output file, and the number or reps |
// | PROCESS:                                              |
// | call the iterative reverse function, number times     |
// | OUTPUT:                                               |
// | The amount of time it took to complete                |
// +=======================================================+
double timeRecursive(char * inFileName, char * outFileName, int numberOfRepititions){
    // Function only works with positive nums.
    if (numberOfRepititions <= 0) return 0.0;

    // If they did Something normal...
    clock_t timer;
    timer = clock();
    int x;
    for (x = 0; x < numberOfRepititions; x ++){
        recursiveReverseFile(inFileName, outFileName);
    }
    timer = clock() - timer;

    double timeAllotted = ((double) timer) / CLOCKS_PER_SEC;
    return timeAllotted;
}


// SAMPLE OUTPUT FOR PROGRAM:
/*
Enter a number of lines for our input file (default 30383):
>>> 30383
Number of runs:
>>> 20
Processing...
+==============================+
| Iterative version: 0.71      |
| Recursive version: 1.78      |
+==============================+

Iteration is faster by 1.07 seconds.
*/
