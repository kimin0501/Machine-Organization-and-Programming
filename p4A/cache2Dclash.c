////////////////////////////////////////////////////////////////////////////////
// This File:        cache2Dclash.c
// Other Files:      cache1D.c, cache2Drows.c, cache2Dcols.c
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

// declare a 2-dimensional global array of integers having dimensions 128 rows x 8 columns
#define ITERATIONS 100
#define ROWS 128
#define COLS 8

int arr2D[ROWS][COLS];

int main() {
    // outermost loop repeats 100 times
    for (int iteration = 0; iteration < ITERATIONS; iteration++) {
        // middle loop iterates over the rows, incrementing by 64 
        for (int row = 0; row < ROWS; row += 64) {
            // innermost loop iterates over the columns of the array
            for (int col = 0; col < COLS; col++) {
                arr2D[row][col] = iteration + row + col;
            }
        }
    }

    return 0;
}

