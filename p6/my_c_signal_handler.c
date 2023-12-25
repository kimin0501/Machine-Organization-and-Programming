///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020-2023 Deb Deppeler based on work by Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission FALL 2023, CS354-deppeler
//
////////////////////////////////////////////////////////////////////////////////
// Main File:        my_c_signal_handler.c
// This File:        my_c_signal_handler.c
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

// initialize the global variable to set the alarm time
int alarm_time = 5;

// initialize the global counter
int counter = 0; 

// SIGALRM Handler
void alarm_handler() {
    // get the current time
    time_t current_time;

    // check for the return value of time()
    if (time(&current_time) == -1) {
        printf("Error getting the current time\n");
        exit(1);
    }

    // check for the return value of ctime()
    char *time_str = ctime(&current_time);
    if (time_str == NULL) {
        printf("Error converting the current time to string\n");
        exit(1);
    }

    // print the current process ID and current time
    printf("PID: %d CURRENT TIME: %s", getpid(), time_str);

    // reset the alarm
    alarm(alarm_time);
}

// SIGUSR1 Handler
void user_handler() {
    // increments a counter for SIGUSR1
    counter++;

    // print that SIGUSR1 was handled
    printf("SIGUSR1 handled and counted!\n");
}

// SIGINT Handler
void keyboard_handler(){
    // print SIGINT was handled, and the number of SIGUSR1 count
    printf("\n");
    printf("SIGINT handled.\n");
    printf("SIGUSR1 was handled %d times. Exiting now.\n", counter);
    exit(0); 
}

int main() {
    // print messages about the code
    printf("PID and time print every 5 seconds.\n");
    printf("Type Ctrl-C to end the program.\n");
    
    // declare and initialize the sigaction structures for each signal 
    struct sigaction alarm_signal;
    memset(&alarm_signal, 0, sizeof(alarm_signal));

    struct sigaction user_signal;
    memset(&user_signal, 0, sizeof(user_signal));

    struct sigaction keyboard_signal;
    memset(&keyboard_signal, 0, sizeof(keyboard_signal));
    
    // set the signal_handler as the handler for each signal
    alarm_signal.sa_handler = alarm_handler;
    user_signal.sa_handler = user_handler;
    keyboard_signal.sa_handler = keyboard_handler;   

    // register the signal handler for SIGALRM, and check for an error
    if(sigaction(SIGALRM, &alarm_signal, NULL) != 0) {
        printf("Error binding SIGALRM handler \n");
        exit(1);
    }

    // register the signal handler for SIGUSR1, and check for an error
    if(sigaction(SIGUSR1, &user_signal, NULL) != 0) {
        printf("Error binding SIGUSR1 handler \n");
        exit(1);
    }

    // register the signal handler for SIGINT, and check for an error
    if(sigaction(SIGINT, &keyboard_signal, NULL) != 0) {
        printf("Error binding SIGINT handler \n");
        exit(1);
    }    

    // set an initial alarm
    alarm(alarm_time);

    // infinite loop to keep the program running
    while(1) {
    }

    return 0;
}
