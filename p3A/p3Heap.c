///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020-2023 Deb Deppeler based on work by Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission FALL 2023, CS354-deppeler
//
////////////////////////////////////////////////////////////////////////////////
// Main File:        p3Heap.c
// This File:        p3Heap.c
// Other Files:      Makefile, tests
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

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include "p3Heap.h"

/*
 * This structure serves as the header for each allocated and free block.
 * It also serves as the footer for each free block.
 */
typedef struct blockHeader {           

    /*
     * 1) The size of each heap block must be a multiple of 8
     * 2) heap blocks have blockHeaders that contain size and status bits
     * 3) free heap block contain a footer, but we can use the blockHeader 
     *.
     * All heap blocks have a blockHeader with size and status
     * Free heap blocks have a blockHeader as its footer with size only
     *
     * Status is stored using the two least significant bits.
     *   Bit0 => least significant bit, last bit
     *   Bit0 == 0 => free block
     *   Bit0 == 1 => allocated block
     *
     *   Bit1 => second last bit 
     *   Bit1 == 0 => previous block is free
     *   Bit1 == 1 => previous block is allocated
     * 
     * Start Heap: 
     *  The blockHeader for the first block of the heap is after skip 4 bytes.
     *  This ensures alignment requirements can be met.
     * 
     * End Mark: 
     *  The end of the available memory is indicated using a size_status of 1.
     * 
     * Examples:
     * 
     * 1. Allocated block of size 24 bytes:
     *    Allocated Block Header:
     *      If the previous block is free      p-bit=0 size_status would be 25
     *      If the previous block is allocated p-bit=1 size_status would be 27
     * 
     * 2. Free block of size 24 bytes:
     *    Free Block Header:
     *      If the previous block is free      p-bit=0 size_status would be 24
     *      If the previous block is allocated p-bit=1 size_status would be 26
     *    Free Block Footer:
     *      size_status should be 24
     */
    int size_status;

} blockHeader;         

/* Global variable - DO NOT CHANGE NAME or TYPE. 
 * It must point to the first block in the heap and is set by init_heap()
 * i.e., the block at the lowest address.
 */
blockHeader *heap_start = NULL;     

/* Size of heap allocation padded to round to nearest page size.
 */
int alloc_size;

/*
 * Additional global variables may be added as needed below
 * TODO: add global variables needed by your function
 */




/* 
 * Function for allocating 'size' bytes of heap memory.
 * Argument size: requested size for the payload
 * Returns address of allocated block (payload) on success.
 * Returns NULL on failure.
 *
 * This function must:
 * - Check size - Return NULL if size < 1 
 * - Determine block size rounding up to a multiple of 8 
 *   and possibly adding padding as a result.
 *
 * - Use BEST-FIT PLACEMENT POLICY to chose a free block
 *
 * - If the BEST-FIT block that is found is exact size match
 *   - 1. Update all heap blocks as needed for any affected blocks
 *   - 2. Return the address of the allocated block payload
 *
 * - If the BEST-FIT block that is found is large enough to split 
 *   - 1. SPLIT the free block into two valid heap blocks:
 *         1. an allocated block
 *         2. a free block
 *         NOTE: both blocks must meet heap block requirements 
 *       - Update all heap block header(s) and footer(s) 
 *              as needed for any affected blocks.
 *   - 2. Return the address of the allocated block payload
 *
 *   Return if NULL unable to find and allocate block for required size
 *
 * Note: payload address that is returned is NOT the address of the
 *       block header.  It is the address of the start of the 
 *       available memory for the requesterr.
 *
 * Tips: Be careful with pointer arithmetic and scale factors.
 */
void* balloc(int size) {     
    //TODO: Your code goes in here.
    
    // check size
    if(size < 1){
        return NULL;
    }

    // total size of block needed
    int totalSize = size + sizeof(blockHeader);

    // round up the size to the nearest multiple of 8
    if(totalSize % 8 != 0){
        totalSize = totalSize - (totalSize % 8) + 8;
    }

    // initialize a pointer for pointing to the start of the heap
    blockHeader *currentBlock = heap_start;
    blockHeader *blockFooter = heap_start;

    // initialize a pointer to find the best fit block
    blockHeader *bestFitBlock = NULL;

    
    // traverse the heap to find the best fit block until the end of the heap
    while(currentBlock -> size_status != 1){
        
        // calculate the current block size
        int currentBlockSize = currentBlock -> size_status - (currentBlock->size_status % 8);
        
        // check if the current block is free and big enough to save memory
        if((currentBlock -> size_status & 1) == 0 && ((currentBlock -> size_status) - (currentBlock -> size_status % 8)) >= totalSize) {
            // check if the current block is best fit
            if(!bestFitBlock || ((currentBlock -> size_status) - (currentBlock -> size_status % 8)) < ((bestFitBlock -> size_status) - (bestFitBlock -> size_status % 8))) {
                bestFitBlock = currentBlock;
            }
            
            // check if the current block size exactly matches with the desired size
            if(currentBlockSize == totalSize){
                break;
            }
        }

        // move the current block pointer to the next block
        currentBlock = (blockHeader*)((char*)currentBlock + ((currentBlock -> size_status) - (currentBlock -> size_status % 8)));
    }

    // check if there is no best fit block 
    if(!bestFitBlock) {
        return NULL;
    }

    // calculate the size of best fit block
    int bestFitBlockSize = bestFitBlock -> size_status - (bestFitBlock->size_status % 8);

    // check if the best fit block size equals the requested size
    if(bestFitBlockSize == totalSize) {
        
        // mark the block as allocated
        bestFitBlock -> size_status |= 1;

        // update the next block that its previous block is allocated
        blockHeader *nextBlock = (blockHeader*)((char*)bestFitBlock + bestFitBlockSize);
        if(nextBlock -> size_status != 1) {
            nextBlock -> size_status |= 2;
        }
        // return a pointer of the block's payload
        return (void*)((char*)bestFitBlock + sizeof(blockHeader));

    } else {
        // calculate the size of the block it would remain after the allocation
        int remainingSize = bestFitBlockSize - totalSize;
        
        // split the block with remaining free block
        bestFitBlock -> size_status = totalSize | 1 | (bestFitBlock -> size_status & 2);

        // create a new free block after newly allocated block and update p-bit
        blockHeader *newFreeBlock = (blockHeader*)((char*)bestFitBlock + totalSize);
        newFreeBlock -> size_status = remainingSize | 2;

        // Add footer for the newly freed block
        blockFooter = (blockHeader*)((char*)newFreeBlock + remainingSize - sizeof(blockHeader));
        blockFooter -> size_status = remainingSize;
            
            //  update the next block that its previous block is free
            blockHeader *nextBlock = (blockHeader*)((char*)newFreeBlock + remainingSize);
            if(nextBlock -> size_status != 1) {
                nextBlock -> size_status &= ~2; 
            }

    // return a pointer of the allocated block payload memory
     return (void*)((char*)bestFitBlock + sizeof(blockHeader));
}
}

/* 
 * Function for freeing up a previously allocated block.
 * Argument ptr: address of the block to be freed up.
 * Returns 0 on success.
 * Returns -1 on failure.
 * This function should:
 * - Return -1 if ptr is NULL.
 * - Return -1 if ptr is not a multiple of 8.
 * - Return -1 if ptr is outside of the heap space.
 * - Return -1 if ptr block is already freed.
 * - Update header(s) and footer as needed.
 *
 * If free results in two or more adjacent free blocks,
 * they will be immediately coalesced into one larger free block.
 * so free blocks require a footer (blockHeader works) to store the size
 *
 * TIP: work on getting immediate coalescing to work after your code 
 *      can pass the tests in partA and partB of tests/ directory.
 *      Submit code that passes partA and partB to Canvas before continuing.
 */                    
int bfree(void *ptr) {    
    //TODO: Your code goes in here.

    // check if ptr is NULL
    if (ptr == NULL) {
        return -1;
    }

    // check if ptr is not a multiple of 8
    if((unsigned int)ptr % 8 != 0){
        return -1;
    }

    // initialize header pointer of currentblock and end of heap
    blockHeader* currentBlock = ptr - sizeof(blockHeader);
    blockHeader* heap_end = (blockHeader*)((char*)heap_start + alloc_size);

    // check if ptr is outside of the heap space
    if (currentBlock < heap_start || currentBlock >= heap_end) {
        return -1;
    }

    // check if ptr is already freed
    if (!(currentBlock -> size_status & 1)) {
        return -1;
    }

    // set the current block as free
    currentBlock -> size_status = currentBlock -> size_status - 1;

    // compute the size of current block
    int currentBlockSize = currentBlock -> size_status - (currentBlock -> size_status % 8);

    // make a footer of newly freed block
    blockHeader* currentFooter = (blockHeader*)((char*)currentBlock + currentBlockSize - sizeof(blockHeader));
    currentFooter -> size_status = currentBlockSize;

    // initialize the header pointer of next block and its size
    blockHeader* nextBlock = (blockHeader*)((char*)currentBlock + currentBlockSize);
    int nextBlockSize;

    // compute the size of the next block
    if (nextBlock >= heap_end) {
        nextBlockSize = 0; 
    } else {
        nextBlockSize = nextBlock -> size_status - (nextBlock->size_status % 8);
    }

    // coalesce the next block with the current block if the condition met
    if (!(nextBlock -> size_status & 1) && nextBlock < heap_end) {
        // update the coalesced block's information
        currentBlockSize += nextBlockSize;
        currentBlock -> size_status = currentBlockSize + (currentBlock -> size_status % 8);

        // update the changed footer information
        currentFooter = (blockHeader*)((char*)currentBlock + currentBlockSize - sizeof(blockHeader));
        currentFooter -> size_status = currentBlockSize;

        // set the p-bit of the new next block as 0 to indicate that its previous block is free
        blockHeader* newNextBlock = (blockHeader*)((char*)currentBlock + currentBlockSize);
        if (newNextBlock < heap_end) {
            newNextBlock -> size_status &= ~2; 
        }
    }

    // coalesce the previous block with the current block if the condition met
    if (currentBlock != heap_start) {
        
        // find the previous block's footer to get its informatiom
        blockHeader* previousFooter = (blockHeader*)((char*)currentBlock - sizeof(blockHeader));
        int prevBlockSize = previousFooter -> size_status;
        blockHeader* previousBlock = (blockHeader*)((char*)currentBlock - prevBlockSize);
        
        // check whether it is free to coalesce
        if (!(previousBlock -> size_status & 1)) { 
            // update the coalesced block's information
            currentBlockSize += prevBlockSize;
            previousBlock -> size_status = currentBlockSize + (previousBlock -> size_status & 2);  
            currentBlock = previousBlock;

            // update the changed footer information
            currentFooter = (blockHeader*)((char*)currentBlock + currentBlockSize - sizeof(blockHeader));
            currentFooter -> size_status = currentBlockSize;
        }
    }

    return 0;
}

/* 
 * Initializes the memory allocator.
 * Called ONLY once by a program.
 * Argument sizeOfRegion: the size of the heap space to be allocated.
 * Returns 0 on success.
 * Returns -1 on failure.
 */                    
int init_heap(int sizeOfRegion) {    

    static int allocated_once = 0; //prevent multiple myInit calls

    int   pagesize; // page size
    int   padsize;  // size of padding when heap size not a multiple of page size
    void* mmap_ptr; // pointer to memory mapped area
    int   fd;

    blockHeader* end_mark;

    if (0 != allocated_once) {
        fprintf(stderr, 
                "Error:mem.c: InitHeap has allocated space during a previous call\n");
        return -1;
    }

    if (sizeOfRegion <= 0) {
        fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
        return -1;
    }

    // Get the pagesize from O.S. 
    pagesize = getpagesize();

    // Calculate padsize as the padding required to round up sizeOfRegion 
    // to a multiple of pagesize
    padsize = sizeOfRegion % pagesize;
    padsize = (pagesize - padsize) % pagesize;

    alloc_size = sizeOfRegion + padsize;

    // Using mmap to allocate memory
    fd = open("/dev/zero", O_RDWR);
    if (-1 == fd) {
        fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
        return -1;
    }
    mmap_ptr = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (MAP_FAILED == mmap_ptr) {
        fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
        allocated_once = 0;
        return -1;
    }

    allocated_once = 1;

    // for double word alignment and end mark
    alloc_size -= 8;

    // Initially there is only one big free block in the heap.
    // Skip first 4 bytes for double word alignment requirement.
    heap_start = (blockHeader*) mmap_ptr + 1;

    // Set the end mark
    end_mark = (blockHeader*)((void*)heap_start + alloc_size);
    end_mark->size_status = 1;

    // Set size in header
    heap_start->size_status = alloc_size;

    // Set p-bit as allocated in header
    // note a-bit left at 0 for free
    heap_start->size_status += 2;

    // Set the footer
    blockHeader *footer = (blockHeader*) ((void*)heap_start + alloc_size - 4);
    footer->size_status = alloc_size;

    return 0;
} 

/* STUDENTS MAY EDIT THIS FUNCTION, but do not change function header.
 * TIP: review this implementation to see one way to traverse through
 *      the blocks in the heap.
 *
 * Can be used for DEBUGGING to help you visualize your heap structure.
 * It traverses heap blocks and prints info about each block found.
 * 
 * Prints out a list of all the blocks including this information:
 * No.      : serial number of the block 
 * Status   : free/used (allocated)
 * Prev     : status of previous block free/used (allocated)
 * t_Begin  : address of the first byte in the block (where the header starts) 
 * t_End    : address of the last byte in the block 
 * t_Size   : size of the block as stored in the block header
 */                     
void disp_heap() {     

    int    counter;
    char   status[6];
    char   p_status[6];
    char * t_begin = NULL;
    char * t_end   = NULL;
    int    t_size;

    blockHeader *current = heap_start;
    counter = 1;

    int used_size =  0;
    int free_size =  0;
    int is_used   = -1;

    fprintf(stdout, 
            "*********************************** HEAP: Block List ****************************\n");
    fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
    fprintf(stdout, 
            "---------------------------------------------------------------------------------\n");

    while (current->size_status != 1) {
        t_begin = (char*)current;
        t_size = current->size_status;

        if (t_size & 1) {
            // LSB = 1 => used block
            strcpy(status, "alloc");
            is_used = 1;
            t_size = t_size - 1;
        } else {
            strcpy(status, "FREE ");
            is_used = 0;
        }

        if (t_size & 2) {
            strcpy(p_status, "alloc");
            t_size = t_size - 2;
        } else {
            strcpy(p_status, "FREE ");
        }

        if (is_used) 
            used_size += t_size;
        else 
            free_size += t_size;

        t_end = t_begin + t_size - 1;

        fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%4i\n", counter, status, 
                p_status, (unsigned long int)t_begin, (unsigned long int)t_end, t_size);

        current = (blockHeader*)((char*)current + t_size);
        counter = counter + 1;
    }

    fprintf(stdout, 
            "---------------------------------------------------------------------------------\n");
    fprintf(stdout, 
            "*********************************************************************************\n");
    fprintf(stdout, "Total used size = %4d\n", used_size);
    fprintf(stdout, "Total free size = %4d\n", free_size);
    fprintf(stdout, "Total size      = %4d\n", used_size + free_size);
    fprintf(stdout, 
            "*********************************************************************************\n");
    fflush(stdout);

    return;  
} 


// end p3Heap.c (Fall 2023)                                         

