///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2021-23 Deb Deppeler
// Posting or sharing this file is prohibited, including any changes/additions.
//
// We have provided comments and structure for this program to help you get 
// started.  Later programs will not provide the same level of commenting,
// rather you will be expected to add same level of comments to your work.
// 09/20/2021 Revised to free memory allocated in get_board_size function.
// 01/24/2022 Revised to use pointers for CLAs
// 
////////////////////////////////////////////////////////////////////////////////
// Main File:        checkboard
// This File:        checkboard
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
//                   Track all work you do in your work log-p2a.pdf
//                   and fully credit all sources of help, including family, 
//                   friends, tutors, Peer Mentors, TAs, and Instructor.
//
// Online sources:   Avoid relying on eeb searches to solve your problems, 
//                   but if you do search, be sure to include Web URLs and 
//                   description of any information you find in your work log.
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *DELIM = ",";  // commas ',' are a common delimiter character for data strings

/* COMPLETED (DO NOT EDIT):       
 * Read the first line of input file to get the size of that board.
 * 
 * PRE-CONDITION #1: file exists
 * PRE-CONDITION #2: first line of file contains valid non-zero integer value
 *
 * fptr: file pointer for the board's input file
 * size: a pointer to an int to store the size
 *
 * POST-CONDITION: the integer whos address is passed in as size (int *) 
 * will now have the size (number of rows and cols) of the board being checked.
 */
void get_board_size(FILE *fptr, int *size) {      
   char *line1 = NULL;
   size_t len = 0;

   if ( getline(&line1, &len, fptr) == -1 ) {
      printf("Error reading the input file.\n");
      free(line1);
      exit(1);
   }

   char *size_chars = NULL;
   size_chars = strtok(line1, DELIM);
   *size = atoi(size_chars);

   // free memory allocated for reading first link of file
   free(line1);
   line1 = NULL;
}



/* TODO:
 * Returns 1 if and only if the board is in a valid Sudoku board state.
 * Otherwise returns 0.
 * 
 * A valid row or column contains only blanks or the digits 1-size, 
 * with no duplicate digits, where size is the value 1 to 9.
 * 
 * Note: p2A requires only that each row and each column are valid.
 * 
 * board: heap allocated 2D array of integers 
 * size:  number of rows and columns in the board
 */
int valid_board(int **board, int size) {
   
    // Create the array that tests whether sudoku valid
    int testSudoku[size];

    //initialize the array with 0
    for (int m = 0; m < size; m++) {
        *(testSudoku + m) = 0;
    }
    
    //Row check
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            // when the value is bigger than size or negative
            if ( *(*(board + i)+j) < 0 || *(*(board + i) + j) > size) {
                return 0;
            }
            // ignore values with 0 and save the frequency of numbers
            if ( *(*(board + i) + j) != 0) {
                *(testSudoku + (*(*(board + i) + j) - 1)) = *(testSudoku + (*(*(board + i) + j) - 1)) + 1;
                // if the incremented value is bigger than 1, invalid
                if (*(testSudoku+(*(*(board + i) + j) - 1)) > 1){
                    return 0;
                }
            }
        }
        
        //initialize again with 0
        for (int m = 0; m < size; m++) {
            *(testSudoku + m) = 0;
        }
    }

   //Column check
    for (int j = 0; j < size; j++) {
        for (int i = 0; i < size; i++) {
            // when the value is bigger than size or negative
            if ( *(*(board + i)+j) < 0 || *(*(board + i) + j) > size) {
                return 0;
            }
            // ignore values with 0 and save the frequency of numbers
            if ( *(*(board + i) + j) != 0) {
                *(testSudoku + (*(*(board + i) + j) - 1)) = *(testSudoku+(*(*(board + i) + j) - 1)) + 1;
                // if the incremented value is bigger than 1, invalid
                if (*(testSudoku+(*(*(board + i) + j) - 1)) > 1){
                    return 0;
                }
            }
        }
        //initialize again with 0
        for (int m = 0; m < size; m++) {
            *(testSudoku + m) = 0;
        }
    }
    // Otherwise, valid   
    return 1;  
}    


/* TODO: COMPLETE THE MAIN FUNCTION
 * This program prints "valid" (without quotes) if the input file contains
 * a valid state of a Sudoku puzzle board wrt to rows and columns only.
 *
 * A single CLA is required, which is the name of the file 
 * that contains board data is required.
 *
 * argc: the number of command line args (CLAs)
 * argv: the CLA strings, includes the program name
 *
 * Returns 0 if able to correctly output valid or invalid.
 * Only exit with a non-zero result if unable to open and read the file given.
 */
int main( int argc, char **argv ) {              

   // TODO: Check if number of command-line arguments is correct.
   if (argc != 2){
      printf("Invalid number of command line arguments.\n");
      exit(1);
   }

   // Open the file and check if it opened successfully.
   FILE *fp = fopen(*(argv + 1), "r");
   if (fp == NULL) {
      printf("Can't open file for reading.\n");
      exit(1);
   }

   // Declare local variables.
   int size;

   // TODO: Call get_board_size to read first line of file as the board size.
   get_board_size(fp , &size);

   // TODO: Dynamically allocate a 2D array for given board size.
   // DO NOT create a 1D array of ints on the stack or heap
   // You must dyamically create a 1D arrays of pointers to other 1D arrays of ints
  
   // Create a 2D array with given size
   int **board = malloc(size * sizeof(int *));

   // Check if the board is NULL
   if (board == NULL) {
      printf("Memory allocation failed!\n");
      exit(1);
   }

   // Check if the value in the board is NULL
   for (int i = 0; i < size; i++) {
      *(board + i) = malloc(size * sizeof(int));
      if (*(board + i) == NULL) {
         printf("Memory allocation failed for row %d!\n", i);
         exit(1);
      }
   }

   // Read the rest of the file line by line.
   // Tokenize each line wrt the delimiter character 
   // and store the values in your 2D array.
   char *line = NULL;
   size_t len = 0;
   char *token = NULL;
   for (int i = 0; i < size; i++) {

      if (getline(&line, &len, fp) == -1) {
         printf("Error while reading line %i of the file.\n", i+2);
         exit(1);
      }

      token = strtok(line, DELIM);
      for (int j = 0; j < size; j++) {
         // TODO: Complete the line of code below
         // to initialize your 2D array.
         /* ADD ARRAY ACCESS CODE HERE */ // atoi(token);
         *(*(board + i) + j) = atoi(token);
         token = strtok(NULL, DELIM);
      }
   }

   // TODO: Call the function valid_board and print the appropriate
   //       output depending on the function's return value.
   if (valid_board(board, size) == 1){
      printf("valid\n");
   } else {
      printf("invalid\n");
   
}
   // TODO: Free all dynamically allocated memory.

for(int i = 0; i < size; i++){
   free(*(board + i));
   *(board + i) = NULL;
}
   
   // free the pointers
   free(board);
   free(line);
   free(token);

   // Prevent dangling pointers
   board = NULL;
   line = NULL;
   token = NULL;

   //Close the file.
   if (fclose(fp) != 0) {
      printf("Error while closing the file.\n");
      exit(1);
   } 

   return 0;      

   }       

