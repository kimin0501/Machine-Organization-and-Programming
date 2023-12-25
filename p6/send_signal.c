///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020-2023 Deb Deppeler based on work by Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission FALL 2023, CS354-deppeler
//
////////////////////////////////////////////////////////////////////////////////
// Main File:        send_signal.c
// This File:        send_signal.c
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
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv){
    
    // check for the number of command line arguments
    if(argc != 3){
        printf("Usage: sendsig <signal type> <pid>\n");
	    exit(1);
    }

    // initialize the signal type and process ID
    int signal;
    int p_id = atoi(argv[2]);
    
    // check if the signal type is valid and update the signal type
    if (strcmp(argv[1], "-i") == 0) {
        signal = SIGINT;
    } else if (strcmp(argv[1], "-u") == 0) {
        signal = SIGUSR1;
    } else {
        printf("Invalid signal type.\n");
        exit(1);
    }
    
    // send the signal to given process ID and check for error
    if (kill(p_id, signal) != 0){
        printf("Error with sending signal");
        exit(1);
    }

    return 0;
}
