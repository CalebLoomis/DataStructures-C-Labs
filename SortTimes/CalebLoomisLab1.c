// Author: Caleb Loomis
// Assignment Number: Lab 2
// File Name: CalebLoomisLab2.c
// Course/Section: CS2123 Section 1
// Date: July 5, 2020
// Instructor: Dr. Ku

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

const char DATA_FILE_NAME[] = "TestData.txt";

typedef struct functionRuntimes
{
    char *name;             //name of the function being tested
    double **times;         //run times
    double *avg;            //average runtime
    int numTestCases;       //number of times to repeat each test size
    int numTestCaseSizes;   //number of test sizes
    int *testCaseSizes;     //array containing the test sizes
} functionRuntimes;

//Functions used to test the runtimes
functionRuntimes timeAlgorithm( char*, int, int, int[], void (*f)(FILE *) );
FILE *generateTestInput( int, int, int );
void computeAvg( functionRuntimes fRT );
void printRuntimeTable( functionRuntimes fRT );
void freeFunctionRuntimes( functionRuntimes fRT );

//Functions whose runtime will be tested (and helper functions)
void insertionSortInitial( FILE* input );
void insertionSort( int* points, int low, int high );
void quickSortOptInitial( FILE* input );
void quickSortOpt( int* points, int low, int high );
int partition( int* points, int low, int high );

void mysteryRuntime1( FILE* input );
void mysteryRuntime2( FILE* input );
void mysteryRuntime3( FILE* input );

/*
 * Provided code - DO NOT CHANGE THIS METHOD
 * (if you make alterations plan to revert them before submission)
 */
int main( int argc, char *argv[] )
{
    functionRuntimes fRT;

    int sizes1[] = { 1000, 2500, 5000, 7500, 10000};
    int sizes2[] = { 10000, 20000, 40000, 80000};

    srand(time(0));

    fRT = timeAlgorithm("Insertion Sort", 10, 5, sizes1, insertionSortInitial );
    printRuntimeTable(fRT);
    freeFunctionRuntimes(fRT);

    fRT = timeAlgorithm("quicksort (uses insertion sort when sorting <30 numbers)", 10, 5, sizes1, quickSortOptInitial );
    printRuntimeTable(fRT);
    freeFunctionRuntimes(fRT);

    fRT = timeAlgorithm("Mystery 1", 8, 4, sizes2, mysteryRuntime1 );
    printRuntimeTable(fRT);
    freeFunctionRuntimes(fRT);

    fRT = timeAlgorithm("Mystery 2", 8, 4, sizes2, mysteryRuntime2 );
    printRuntimeTable(fRT);
    freeFunctionRuntimes(fRT);

    fRT = timeAlgorithm("Mystery 3", 8, 4, sizes2, mysteryRuntime3 );
    printRuntimeTable(fRT);
    freeFunctionRuntimes(fRT);

    return 0;
}

/*************************************** Functions to have their runtimes tested *********************************************/

/* provided code - DO NOT CHANGE
 */
void mysteryRuntime1( FILE* input )
{
    int temp;
    int size;
    int i=0;
    int *array;

    if( fscanf( input, "%d", &size ) != 1 )
    {
        exit(-1);
    }
    array = (int *) malloc( size*sizeof(int) );
    if( array == NULL )
    {
        exit(-1);
    }

    while( fscanf( input, "%d", &temp ) == 1 && i<size)
    {
        array[i] = temp;
        i++;
    }

    while( size>1 )
    {
        size = size/2;
        array[size/2] = array[size];
    }

    free(array);
}

/* provided code - DO NOT CHANGE
 */
void mysteryRuntime2( FILE* input )
{
    int temp;
    int size;
    int i=0, j=0;
    int *array;

    if( fscanf( input, "%d", &size ) != 1 )
    {
        exit(-1);
    }
    array = (int *) malloc( size*sizeof(int) );
    if( array == NULL )
    {
        exit(-1);
    }

    while( fscanf( input, "%d", &temp ) == 1 && i<size)
    {
        array[i] = temp;
        i++;
    }

    i=0;
    while( j<size )
    {
        array[j] = array[i];

        i++;
        if( i>=size )
        {
            j++;
            i=0;
        }
    }

    free(array);
}

/* provided code - DO NOT CHANGE
 */
void mysteryRuntime3( FILE* input )
{
    int temp;
    int size;
    int n;
    int i=0;
    int *array;

    if( fscanf( input, "%d", &size ) != 1 )
    {
        exit(-1);
    }
    array = (int *) malloc( size*sizeof(int) );
    if( array == NULL )
    {
        exit(-1);
    }

    while( fscanf( input, "%d", &temp ) == 1 && i<size)
    {
        array[i] = temp;
        i++;
    }

    for( i=0; i<size; i++ )
    {
        for( n=size-1; n>1; n/=1.01 )
        {
            array[n-1] = array[n];
        }
    }

    free(array);
}


/*
 * Provided code - DO NOT CHANGE THIS METHOD
 */
void insertionSortInitial( FILE* input )
{
    int i;
    int size;
    int *array;

    fscanf( input, "%d", &size );
    array = (int *) malloc( size*sizeof(int) );

    for( i=0; i<size; i++)
    {
        fscanf( input, "%d", &array[i] );
    }

    insertionSort( array, 0, size-1 );

    //Error check to verify the array is sorted
    /*for( i=1; i<size; i++)
    {
        if(array[i-1]>array[i])
        {
            printf("Not sorted!");
            exit(-1);
        }
    }*/

    free(array);
}

/*
 * Provided code - DO NOT CHANGE THIS METHOD
 */
void insertionSort( int* points, int low, int high )
{
    int i, j;
    double temp;

    for( i = low+1; i <= high; i++)
    {
        for( j = i; j > low && points[j]<points[j-1]; j--)
        {
            temp = points[j];
            points[j] = points[j-1];
            points[j-1] = temp;
        }
    }
}

/*
 * Provided code - DO NOT CHANGE THIS METHOD
 */
void quickSortOptInitial( FILE* input )
{
    int i;
    int size;
    int *array;

    fscanf( input, "%d", &size );
    array = (int *) malloc( size*sizeof(int) );

    for( i=0; i<size; i++)
    {
        fscanf( input, "%d", &array[i] );
    }

    quickSortOpt( array, 0, size-1 );

    //Error check to verify the array is sorted
    /*for( i=1; i<size; i++)
    {
        if(array[i-1]>array[i]){
            printf("Not sorted!");
            exit(-1);
        }
    }*/

    free(array);
}

/*
 * Provided code - DO NOT CHANGE THIS METHOD
 */
void quickSortOpt( int* points, int low, int high )
{
    if( high < low+30 )
    {
        insertionSort( points, low, high );
    }
    else
    {
        int pivot = partition( points, low, high );

        quickSortOpt( points, low, pivot-1 );
        quickSortOpt( points, pivot+1, high );
    }
}

/*
 * Provided code - DO NOT CHANGE THIS METHOD
 */
int partition( int* points, int low, int high )
{
    int pivot = rand() % (high - low + 1) + low;
    int pivotValue = points[pivot];
    int i = low+1;
    int j = high;
    int temp;

    points[pivot] = points[low];
    points[low] = pivotValue;

    while( i<j )
    {
        while( i<=high && points[i] <= pivotValue )
        {
            i++;
        }
        while( j>=low && points[j] > pivotValue )
        {
            j--;
        }
        if(i<j)  //swap out of order elements
        {
            temp = points[i];
            points[i] = points[j];
            points[j] = temp;
        }
    }
    if( i<=high && points[i] <= pivotValue )
    {
        i++;
    }

    points[low] = points[i-1];
    points[i-1] = pivotValue;

    return i-1;
}

/*************************************** Functions for finding and printing runtime *********************************************/

/*
Give your asymptotic estimates for the runtimes of the following 3 functions:

mysteryRuntime1:  O(n)
mysteryRuntime2:  O(2^n)
mysteryRuntime3:  O(n^2)
*/


/* TO BE COMPLETED BY YOU
 * Fill in the missing parts of the code (see TODOs below)
 */
functionRuntimes timeAlgorithm( char *name, int numTestCases, int numTestCaseSizes, int testCaseSizes[], void (*f)(FILE *) )
{
    /* Call and calculate the runtime of the provided function f */
    clock_t start, end;
    int i, j;
    FILE *testData;

    //create functionRuntimes variable to return
    functionRuntimes fRT;
    //Copy passed data into the fRT variable
    fRT.name = name;
    fRT.numTestCases = numTestCases;
    fRT.numTestCaseSizes = numTestCaseSizes;
    fRT.testCaseSizes = testCaseSizes;

    //TODO: initialize and fill in the runtimes table
    fRT.times = (double **) malloc( numTestCaseSizes*sizeof(double *) );
    fRT.avg = (double *) malloc(numTestCaseSizes*sizeof(double));
    for( i=0; i<numTestCaseSizes; i++ )
    {
        fRT.times[i] = (double *) malloc( numTestCases*sizeof(double) );

        for( j=0; j<numTestCases; j++ )
        {
            //Generate test data for the function f
            testData = generateTestInput( 0, testCaseSizes[i], testCaseSizes[i] );

            //Run f on the generated test data
            start = clock();
            f( testData );
            end = clock();
            fclose(testData);

            //Enter the elapsed number of seconds into the times array for fRT
            fRT.times[i][j] = (double)(end - start) / CLOCKS_PER_SEC;
        }
    }

    //TODO: Calculate the average runtimes (call computeAvg here)
    computeAvg(fRT);
    return fRT;
}

/*
 * Provided code - DO NOT CHANGE THIS METHOD
 */
FILE *generateTestInput( int min, int max, int size )
{
    int i;
    FILE *data = fopen( DATA_FILE_NAME, "w" );

    if( data==NULL )
    {
        printf("Failed to create file %s\n", DATA_FILE_NAME);
        exit(-1);
    }

    //add size to start of file
    fprintf( data, "%d ", size );
    //Fill the file with random data
    for( i=0; i<size; i++ )
    {
        fprintf( data, "%d ", rand()%(max-min+1)+min );
    }
    fclose(data);

    data = fopen( DATA_FILE_NAME, "r" );
    if( data==NULL )
    {
        printf("Failed to create file %s\n", DATA_FILE_NAME);
        exit(-1);
    }
    return data;
}

/* TO BE COMPLETED BY YOU
 * Calculate and insert the average runtime for each set of test data into fRT
 */
void computeAvg( functionRuntimes fRT )
{
    int totalCases;
    double totalTime;
    int i, j;

    //Go through each test case
    for (i = 0; i < fRT.numTestCaseSizes; i++){
        totalCases = fRT.numTestCases;
        totalTime = 0;

        for (j = 0; j < fRT.numTestCases; j++){
            totalTime += fRT.times[i][j];
        }
        //Assign avg
        fRT.avg[i] = (double) totalTime/totalCases;
    }
}

/* TO BE COMPLETED BY YOU
 * Print the information in fRT as a 2d table.  Display 3 digits after the decimal point.  You can assume all of the runtimes are <= 99.999 seconds.
 */
void printRuntimeTable( functionRuntimes fRT )
{
    int i, j;
    // Make table header
    printf("%s\n", fRT.name);
    printf("%s\t", "Size");
    for (i = 0; i < fRT.numTestCases; i++){
        printf("Test #%i\t", i);
    }
    printf("Average\n" );

    //Go through each test case
    for (i = 0; i < fRT.numTestCaseSizes; i++){
        printf("%i\t", fRT.testCaseSizes[i]);
        //Go through each test and print the time
        for (j = 0; j < fRT.numTestCases; j++){
            printf("%.3f\t", fRT.times[i][j]);
        }
        //Print avg last.
        printf("%.3f\n", fRT.avg[i]);
    }
    //Add a newline after the table just so the potential next isn't right below
    printf("\n");
}

/* TO BE COMPLETED BY YOU
 * Free all of the dynamically allocated memory in fRT
 */
void freeFunctionRuntimes( functionRuntimes fRT )
{
    int i;
    //char *name;     never malloc'd
    for (i = 0; i < fRT.numTestCaseSizes; i++ )
    {
        free(fRT.times[i]); // Because each row was malloc'd we need to free it.
    }
    free(fRT.times);          //2d array of times
    free(fRT.avg);            //average runtime
    //int numTestCases;       //never malloc'd
    //int numTestCaseSizes;   //never malloc'd
    //int *testCaseSizes;     //never malloc'd
}

/*
SAMPLE OUTPUT
=============================================================================
Insertion Sort
Size	Test #0	Test #1	Test #2	Test #3	Test #4	Test #5	Test #6	Test #7	Test #8	Test #9	Average
1000	0.003	0.002	0.002	0.002	0.002	0.002	0.002	0.002	0.002	0.002	0.002
2500	0.014	0.012	0.013	0.012	0.012	0.013	0.015	0.015	0.013	0.012	0.013
5000	0.048	0.055	0.048	0.053	0.050	0.049	0.054	0.048	0.055	0.048	0.051
7500	0.113	0.115	0.117	0.110	0.114	0.115	0.116	0.117	0.117	0.116	0.115
10000	0.207	0.204	0.194	0.202	0.200	0.205	0.199	0.199	0.199	0.199	0.201

quicksort (uses insertion sort when sorting <30 numbers)
Size	Test #0	Test #1	Test #2	Test #3	Test #4	Test #5	Test #6	Test #7	Test #8	Test #9	Average
1000	0.000	0.000	0.000	0.000	0.000	0.000	0.000	0.000	0.000	0.000	0.000
2500	0.001	0.001	0.001	0.001	0.001	0.001	0.001	0.001	0.001	0.001	0.001
5000	0.001	0.001	0.001	0.001	0.001	0.001	0.001	0.001	0.001	0.001	0.001
7500	0.002	0.002	0.002	0.002	0.002	0.002	0.002	0.002	0.002	0.002	0.002
10000	0.002	0.002	0.002	0.002	0.002	0.002	0.002	0.002	0.002	0.003	0.002

Mystery 1
Size	Test #0	Test #1	Test #2	Test #3	Test #4	Test #5	Test #6	Test #7	Average
10000	0.001	0.001	0.001	0.001	0.001	0.001	0.001	0.001	0.001
20000	0.004	0.004	0.002	0.002	0.002	0.002	0.002	0.002	0.003
40000	0.004	0.004	0.004	0.004	0.004	0.004	0.004	0.004	0.004
80000	0.008	0.008	0.008	0.008	0.008	0.011	0.009	0.008	0.008

Mystery 2
Size	Test #0	Test #1	Test #2	Test #3	Test #4	Test #5	Test #6	Test #7	Average
10000	0.265	0.261	0.264	0.262	0.261	0.260	0.264	0.262	0.262
20000	1.023	1.015	1.013	1.023	1.019	1.020	1.013	1.016	1.018
40000	4.051	4.087	4.253	4.338	4.329	4.349	4.327	4.300	4.254
80000	17.222	17.354	17.275	17.264	17.244	17.210	17.310	17.365	17.280

Mystery 3
Size	Test #0	Test #1	Test #2	Test #3	Test #4	Test #5	Test #6	Test #7	Average
10000	0.063	0.054	0.055	0.061	0.058	0.061	0.056	0.063	0.059
20000	0.135	0.136	0.132	0.133	0.135	0.136	0.133	0.133	0.134
40000	0.295	0.283	0.287	0.284	0.284	0.285	0.283	0.287	0.286
80000	0.627	0.631	0.629	0.625	0.623	0.624	0.680	0.706	0.643

*/
