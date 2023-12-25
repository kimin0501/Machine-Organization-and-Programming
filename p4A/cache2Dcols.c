////////////////////////////////////////////////////////////////////////////////
// This File:        cache2Dcols.c
// Other Files:      cache1D.c, cache2Drows.c, cache2Dclash.c
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

// declare a 2-dimensional global array of integers having dimensions 3000 rows x 500 columns
#define ROWS 3000
#define COLS 500

int arr2D[ROWS][COLS];

int main() {
    // outer loop iterate through the columns of the array
    for (int col = 0; col < COLS; col++) {
        // inner loop iterate through the elements of a single column of the array
        for (int row = 0; row < ROWS; row++) {
            arr2D[row][col] = row + col;
        }
    }

    return 0;
}