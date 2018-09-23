#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "sudoku_pthreads.h"

// Defined structure that holds parameters for thread
typedef struct
{
    int sf_index;
    int *input_array;
} parameters;

int sudoku_fail[27]; // initialise array for saving result of checks

int checkSudoku_pthreads(int *S)
{    
    pthread_t pthd[27];
    // check rows (row first element indexes increment by 9)
    for (int r = 0, check_index = 0; r < 73; r += 9, check_index++)
    {
	int row[9]; // initialise array to store row
	int rowIndex = 0;
	int rowEnd = r + 9; // set row index limit
	for (int c = r; c < rowEnd; c++) // traverse each element in row
	{
	    row[rowIndex] = S[c];
	    rowIndex++;
	}
	// initialise parameters
	parameters row_data;
	row_data.sf_index = check_index;
	row_data.input_array = row;
	pthread_create(&pthd[check_index], NULL, checkArray_pthreads, (void *)&row_data);
    }

    // checks columns (column first element indexes increment by 1)
    for (int c = 0, check_index = 9; c < 9; c++, check_index++)
    {
	int col[9]; // initialise array to store column
	int colIndex = 0;
	int colEnd = c + 73; // set column index limit
	// pthread_t pthd1;
	for (int r = c; r < colEnd; r += 9) // traverse each element in col
	{
	    col[colIndex] = S[r];
	    colIndex++;
	}
	// initialise parameters
	parameters col_data;
	col_data.sf_index = check_index;
	col_data.input_array = col;
	pthread_create(&pthd[check_index], NULL, checkArray_pthreads, (void *)&col_data);
    }

    // checks grids (column-major)
    // the first 2 for loops traverse first elements of the 9 3x3 grids
    for (int gx = 0, check_index = 18; gx < 7; gx += 3) /* indexes of grids
    							 increment by 3 in x
    							 direction */
    {
    	int gridEnd = gx + (27 * 2); /* index of first element of last 3x3 grid
    					in column */
    	for (int gy = gx; gy < (gridEnd + 1); gy += 27) /* corner indexes
    							   increment by 27 in y
    							   direction */
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
	    // initialise parameters
	    parameters grid_data;
	    grid_data.sf_index = check_index;
	    grid_data.input_array = grid;
	    pthread_create(&pthd[check_index], NULL, checkArray_pthreads, (void *)&grid_data);
    	    check_index++;
    	}
    }

    for (int i = 0; i < 27; i++)
    {
	pthread_join(pthd[i], NULL);
    }
    // check array of sudoku_fail for any 1s
    int result = 0;
    for (int i = 0; i < 27; i++)
    {
	result = result || sudoku_fail[i]; // returns 1 if any invalid solution
    }

    printf("%s\n", result? "Sudoku invalid" : "Sudoku valid");
    return 0;
}

// Function to check validity of a row in a sudoku solution
void *checkArray_pthreads(void *arg)
{
    parameters *param = arg;
    int index = param->sf_index;
    int *n = param->input_array;
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
       checker array. If the counter is not 9, then 1-9 hasn't been 
       matched and that means the solution isn't valid */
    if (counter != 9)
    {
	sudoku_fail[index] = 1;
	pthread_exit(0);
    }
    // we return 1 if the solution is invalid and 0 if it is valid.
    sudoku_fail[index] = 0;
    pthread_exit(0);
}
