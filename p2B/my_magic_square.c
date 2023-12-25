///////////////////////////////////////////////////////////////////////////////
// Copyright 2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission, CS 354 Spring 2022, Deb Deppeler
////////////////////////////////////////////////////////////////////////////////
// Main File:        my_magic_square
// This File:        my_magic_square
// Other Files:      (name of all other files if any)
// Semester:         CS 354 Fall 2023
// Instructor:       
//
// Author:           Ki Min Kang
// Email:            kkang49@wisc.edu
// CS Login:         ki
// GG#:              8
//                   (See https://canvas.wisc.edu/groups for your GG number)
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   Track all work you do in your work log.pdf
//                   and fully credit all sources of help, including family, 
//                   friends, tutors, Peer Mentors, TAs, and Instructor.
//
////////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure that represents a magic square
typedef struct {
    int size;           // dimension of the square
    int **magic_square; // pointer to heap allocated magic square
} MagicSquare;

/* TODO:
 * Promps the user for the magic square's size, read it,
 * check if it's an odd number >= 3 (if not, display the required
 * error message and exit)
 *
 * return the valid number
 */
int getSize() {
    int s_size = 0;
    
    // let the user input the size of square with given condition
    printf("Enter magic square's size (it must be odd integer and >=3)\n");
    scanf("%i", &s_size);

    // check whether the square size is odd and greater than 3
    if(s_size % 2 == 1 && s_size >= 3){
        return s_size;
    } else {
        printf("magic square's size must be odd integer and greater than 3");
        exit(1);
    }
 
} 
   
/* TODO:
 * Makes a magic square of size n,
 * and stores it in a MagicSquare (on the heap)
.*
 * It may use the Siamese magic square algorithm 
 * or alternate from assignment 
 * or another valid algorithm that produces a magic square.
 *
 * n - the number of rows and columns
 *
 * returns a pointer to the completed MagicSquare struct.
 */
MagicSquare *generateMagicSquare(int n) {
   
    // create a struct of board and allocates memory with malloc 
    MagicSquare *board = malloc(sizeof(MagicSquare));
    
    board -> size = n;
    board -> magic_square = malloc(sizeof(int*) * n);

    for(int i = 0; i < n; i++) {
        *(board -> magic_square + i) = malloc(sizeof(int) * n);
    }

    //initialize every element of board to 0 to check whether it is empty
    for(int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            *(*(board -> magic_square + i) + j) = 0;
        }
    }
     
     int totalNum = n * n;
     int currentRow = 0;
     int currentCol = (n - 1) / 2;
     int num = 1;

    // create magic square with given size
    // allocate board elements until every element is full
     while (num <= totalNum){
        
        // Put 1 in the middle of the first row.
        *(*(board -> magic_square + currentRow) + currentCol) = num;

        // From 2, move one space to the left and one space above and 
        // put next element.
        int nextRow = (currentRow - 1 + n) % n;
        int nextCol = (currentCol + 1) % n;

        if (*(*(board -> magic_square + nextRow) + nextCol) == 0){
            currentRow = nextRow;
            currentCol = nextCol;
        } else {
            // If the value is already in the next slot, move down one space 
            // and put next element.
            currentRow = (currentRow + 1) % n;
        }

        num++;
     }

    return board;    
} 

/* TODO:  
 * Opens a new file (or overwrites the existing file)
 * and writes the magic square values to the file
 * in the specified format.
 *
 * magic_square - the magic square to write to a file
 * filename - the name of the output file
 */
void fileOutputMagicSquare(MagicSquare *magic_square, char *filename) {

// open the file with write mode
FILE* fp = fopen(filename, "w");

fprintf(fp, "Size %d\n", magic_square -> size);

// write every element of magic square into file
 for (int i = 0; i < magic_square -> size; i++) {
        for (int j = 0; j < magic_square -> size; j++) {
            fprintf(fp, "%d", *(*(magic_square -> magic_square + i) + j));
            
            // seperate numbers by columns
            if (j < magic_square -> size - 1) {
            fprintf(fp, ",");
            }
        }
        fprintf(fp, "\n");
    }
    // close the file
    fclose(fp);
}


/* TODO:
 * Generates a magic square of the user specified size and
 * outputs the square to the output filename.
 * 
 * Add description of required CLAs here
 */
int main(int argc, char **argv) {
    // TODO: Check input arguments to get output filename

    FILE* fp = fopen(*(argv + 1), "w");

    if (argc != 2){
        printf("Usage: ./my_magic_square <output_filename>");
        exit(1);
    }
    
    if (fp == NULL){
         printf("File should be exist!");
         exit(1);
    }

    fclose(fp);

    // TODO: Get magic square's size from user
    int size = getSize();

    // TODO: Generate the magic square by correctly interpreting 
    //       the algorithm(s) in the write-up or by writing or your own.  
    //       You must confirm that your program produces a 
    //       Magic Sqare as described in the linked Wikipedia page.
     MagicSquare* square = generateMagicSquare(size);

    // TODO: Output the magic square
     
    fileOutputMagicSquare(square, *(argv + 1));

    printf("Magic square has been successfully produced into %s.\n", *(argv + 1));

    // Free all dynamically allocated memory.
    for (int i = 0; i < size; i++) {
        free(*(square -> magic_square + i));
        *(square -> magic_square + i) = NULL;
    }

    free(square -> magic_square);
    free(square);

    // allcate every element null to prevent leak memory
    square -> magic_square = NULL;
    square = NULL;

    return 0;
} 

//    F23