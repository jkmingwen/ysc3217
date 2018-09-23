#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include "sudoku_proc.h"
    
int checkSudoku_proc(int *S)
{
    // allocate shared memory
    int shm_id = shmget(19, (27 * sizeof(int)), IPC_CREAT | 0666);
    if (shm_id < 0) // catch failures
    {
	perror("shmget");
	exit(EXIT_FAILURE);
    }
    
    // attach variable to shared memory
    int *sudoku_fail = (int *) shmat (shm_id, NULL, 0); /* sharedmem variable, 
							   address,
							   isreadonly */
    
    // check rows (row first element indexes increment by 9)
    for (int r = 0, shm_index = 0; r < 73; r += 9, shm_index++)
    {
	pid_t pid;
	pid = fork(); // initialise child process
	
	if (pid < 0) // catch failures
	{
	    perror("fork");
	    exit(EXIT_FAILURE);
	}
	else if (pid == 0) // child process
	{
	    int row[9]; // initialise array to store row
	    int rowIndex = 0;
	    int rowEnd = r + 9; // set row index limit
	    for (int c = r; c < rowEnd; c++) // traverse each element in row
	    {
		row[rowIndex] = S[c];
		rowIndex++;
	    }
	    sudoku_fail[shm_index] = checkArray_proc(row); // fill array with results
	    shmdt(sudoku_fail); // detatch from shared memory
	    exit(EXIT_SUCCESS); // terminate process
	}
    }

    // checks columns (column first element indexes increment by 1)
    for (int c = 0, shm_index = 9; c < 9; c++, shm_index++)
    {
	pid_t pid1;
	pid1 = fork(); // initialise child process
	if (pid1 < 0) // catch failures
	{
	    perror("fork");
	    exit(EXIT_FAILURE);
	}
	else if (pid1 == 0) // child process
	{
	    int col[9]; // initialise array to store column
	    int colIndex = 0;
	    int colEnd = c + 73; // set column index limit
	    for (int r = c; r < colEnd; r += 9) // traverse each element in col
	    {
		col[colIndex] = S[r];
		colIndex++;
	    }
	    sudoku_fail[shm_index] = checkArray_proc(col); // fill array with results
	    shmdt(sudoku_fail); // detatch from shared memory
	    exit(EXIT_SUCCESS); // terminate process
	}
    }

    // checks grids (column-major)
    // the first 2 for loops traverse first elements of the 9 3x3 grids
    for (int gx = 0, shm_index = 18; gx < 7; gx += 3) /* indexes of grids 
							 increment by 3 in x
							 direction */
    {
	int gridEnd = gx + (27 * 2); /* index of first element of last 3x3 grid
					in column */
	for (int gy = gx; gy < (gridEnd + 1); gy += 27) /* corner indexes 
							   increment by 27 in y
							   direction */
	{
	    pid_t pid2;
	    pid2 = fork(); // initialise child process
	    if (pid2 < 0) // catch failures
	    {
		perror("fork");
		exit(EXIT_FAILURE);
	    }
	    else if (pid2 == 0) // child process
	    {
		int grid[9]; // initialise array to store grid
		int gridIndex = 0;
		int gridRowEnd = gy + (1 * 2); // end of row of current 3x3 grid
		//traverse grid row-wise (by 1)
		for (int gxx = gy; gxx < (gridRowEnd + 1); gxx++)
		{
		    int gridColEnd = gxx + (9 * 2); /* end of column of 
						       the current grid */
		    // now we can fill up the grid array (column-major)
		    for (int gyy = gxx; gyy < (gridColEnd + 1); gyy += 9)
		    {
			grid[gridIndex] = S[gyy];
			gridIndex++;
		    }
		}
		sudoku_fail[shm_index] = checkArray_proc(grid);
		shmdt(sudoku_fail); // detatch from shared memory
		exit(EXIT_SUCCESS); // terminate processs
	    }
	    shm_index++;
	}
    }

    for (int i = 0; i < 27; i++)
    {
	wait(NULL); // call wait for all processes to remove from process table
    }

    // check array of sudoku_fail for any 1s
    int result = 0;
    for (int i = 0; i < 27; i++)
    {
	result = result || sudoku_fail[i]; // returns 1 if any invalid solution
    }

    printf("%s\n", result? "Sudoku invalid" : "Sudoku valid");
    return result;
}

// Function to check validity of a row in a sudoku solution
int checkArray_proc(int n[])
{
    int counter = 0;

    for (int j = 0; j < 9; j++) // this acts as our checker array
    {
	for (int i = 0; i < 9; i++)
	{
	    if (n[i] == j + 1)
	    {
		counter++; /* once a number from 1 (out of 9) is matched,
			      the break function breaks out of the for loop
			      and 2 is checked (and so on) */
		break;
	    }
	}
    }
    /* Now we can check if all numbers have been matched using the
       checker array. If adding the value of checker to oneToNine
       does not equal to 0, then that means the solution isn't valid */
    if (counter != 9)
    {
	return 1;
    }

/* Finally, we return 1 if the solution
   is invalid and 0 if it is valid. */
    return 0;
}
