// Author: Caleb Loomis
// Assignment Number: Lab 1 Part A
// File Name: CalebLoomisLab1.zip
// Course/Section: CS2123 Section 1
// Date: May 28, 2020
// Instructor: Dr. Ku

#include <stdio.h>  // printf & scanf

double power(double base, int exponent);

int main(){
    double base;
    int exponent;
    // Input the base and expontent
    printf("Enter a base: \n>>>");
    scanf("%lf", &base);
    printf("Enter an exponent: \n>>>");
    scanf("%d", &exponent);
    double result = power(base, exponent); //We need this as a variable to check if >= 1 for printf.
    if (result >= 1) { //if it's greater than 1, we case less about the training 0s.
        printf("%.0f to the %d power: %.0f\n",base, exponent, power(base, exponent));
    } else { // If its a fraction, we will include trailing 0s.
        printf("%.0f to the %d power: %g\n",base, exponent, power(base, exponent));
    }
}

// +====================== IPO CHART ======================+
// | INPUT:                                                |
// | Base double, exponent int                             |
// | PROCESS:                                              |
// | If exponent is 0, return one. Return base*this with   |
// | exponent - 1 if positive. If negative, call 1/ this   |
// | with a positive exponent.                             |
// | OUTPUT:                                               |
// | The amount of time it took to complete                |
// +=======================================================+
double power(double base, int exponent){
    // a^0 = 1. When we get to this point, we're done
    if (exponent == 0){
        return 1;
    } else if (exponent > 0){ // If we're at a positive, we will multiply our b>
        return base * power(base, exponent-1);
    } else { //if it's a negative number, we'll return 1/base^-exponent
        return 1/power(base, -exponent);
    }
}

/*+========SAMPLE OUTPUT==========+
  | TRIAL 1:                      |
  |-------------------------------|
  | Enter a base:                 |
  | >>>4                          |
  | Enter an exponent:            |
  | >>>4                          |
  | 4 to the 4 power: 256         |
  | TRIAL 2:                      |
  |-------------------------------|
  | Enter a base:                 |
  | >>>4                          |
  | Enter an exponent:            |
  | >>>-1                         |
  | 4 to the -1 power: 0.25       |
  +===============================+*/
