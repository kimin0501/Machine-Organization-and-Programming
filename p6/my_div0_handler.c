///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020-2023 Deb Deppeler based on work by Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission FALL 2023, CS354-deppeler
//
////////////////////////////////////////////////////////////////////////////////
// Main File:        my_div0_handler.c
// This File:        my_div0_handler.c
// Semester:         CS 354 Fall 2023
// Instructor:       Debra Deppeler
//
// Author:           Ki Min Kang
// Email:            kkang49@wisc.edu
// CS Login:         ki
// GG#:              8
//                   (See https://canvas.wisc.edu/groups for your GG number)
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   
//
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <string.h> 
#include <stdlib.h>

// initialize the global counter
int counter = 0;

// SIGFPE Handler
void div0_handler() {
    // print the information after SIGFPE was handled
    printf("Error: a division by 0 operation was attempted.\n");
    printf("Total number of operations completed successfully: %d\n", counter);
    printf("The program will be terminated.\n");
    exit(0);
}

// SIGINT Handler
void keyboard_handler() {
    // print the information after SIGINT was handled
    printf("\n");
    printf("Total number of operations completed successfully: %d\n", counter);
    printf("The program will be terminated.\n");
    exit(0);
}

int main() {
    
    // declare and initialize the sigaction structures for each signal 
    struct sigaction div0_signal;
    memset(&div0_signal, 0, sizeof(div0_signal));
    
    struct sigaction keyboard_signal;
    memset(&keyboard_signal, 0, sizeof(keyboard_signal));

    // set the signal_handler as the handler for each signal
    div0_signal.sa_handler = div0_handler;
    keyboard_signal.sa_handler = keyboard_handler;

    // register the signal handler for SIGFPE, and check for an error
    if(sigaction(SIGFPE, &div0_signal, NULL) != 0) {
        printf("Error binding SIGFPE handler\n");
        exit(1);
    }

    // register the signal handler for SIGINT, and check for an error
    if(sigaction(SIGINT, &keyboard_signal, NULL) != 0) {
        printf("Error binding SIGINT handler\n");
        exit(1);
    }

    // implement divison with an infinite loop
    while(1) {
        // initialize variables to implement division
        char buffer[100];
        int int1, int2;
        
        // user to input the first integer
        printf("Enter first integer: ");
        // check for the input
        if (fgets(buffer, 100, stdin) == NULL) {
            printf("Error reading input\n");
        }
        // covert it into integer
        int1 = atoi(buffer);

        // user to input the second integer
        printf("Enter second integer: ");
        // check for the input
        if (fgets(buffer, 100, stdin) == NULL) {
            printf("Error reading input\n");
        }
        // covert it into integer
        int2 = atoi(buffer);
        
        // check if a division by 0 operation was attempted
        if(int2 == 0) {
            raise(SIGFPE);
        }

        // compute the quotient and remainder
        int quotient = int1 / int2;
        int remainder = int1 % int2;

        printf("%d / %d is %d with a remainder of %d\n", int1, int2, quotient, remainder);
        // increments a counter for the number of successful operations
        counter++;
    }
}
