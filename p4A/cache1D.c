////////////////////////////////////////////////////////////////////////////////
// This File:        cache1D.c
// Other Files:      cache2Drows.c, cache2Dcols.c, cache2Dclash.c
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

// declare a global array of 100,000 integers
#define GLOBAL_N 100000
int arr[GLOBAL_N];

int main() {
    // set the value of each element in the array to its index
    for (int i = 0; i < GLOBAL_N; i++) {
        arr[i] = i;
    }

    return 0;
}