#include <stdio.h>
#include "sudoku_seq.h"
#include "sudoku_proc.h"
#include "sudoku_pthreads.h"
#include "sudoku_openmp.h"

int main()
{
    // valid solution
    int sudoku_valid[9][9] =
	{ {6, 2, 4, 5, 3, 9, 1, 8, 7},
	  {5, 1, 9, 7, 2, 8, 6, 3, 4},
	  {8, 3, 7, 6, 1, 4, 2, 9, 5},
	  {1, 4, 3, 8, 6, 5, 7, 2, 9},
	  {9, 5, 8, 2, 4, 7, 3, 6, 1},
	  {7, 6, 2, 3, 9, 1, 4, 5, 8},
	  {3, 7, 1, 9, 5, 6, 8, 4, 2},
	  {4, 9, 6, 1, 8, 2, 5, 7, 3},
	  {2, 8, 5, 4, 7, 3, 9, 1, 6} };

    // invalid solution due to repeated numbers
    int sudoku_invalid[9][9] =
	{ {6, 2, 4, 5, 3, 9, 1, 8, 7},
	  {5, 1, 9, 7, 2, 8, 6, 3, 4},
	  {8, 3, 7, 6, 1, 4, 2, 9, 5},
	  {1, 4, 3, 8, 6, 5, 7, 2, 9},
	  {9, 5, 8, 2, 4, 7, 3, 5, 1},
	  {7, 6, 2, 3, 9, 1, 4, 5, 8},
	  {3, 7, 1, 9, 5, 6, 8, 4, 2},
	  {4, 9, 6, 1, 8, 2, 5, 7, 3},
	  {2, 8, 5, 4, 7, 3, 9, 5, 6} };

    // invalid solution due to numbers outside of 1-9
    int sudoku_invalid_range[9][9] =
	{ {6, 2, 4, 5, 3, 9, 1, 8, 7},
	  {5, 1, 9, 7, 2, 8, 6, 3, 4},
	  {8, 3, 7, 6, 1, 4, 2, 9, 5},
	  {1, 4, 3, 8, 6, 10, 7, 2, 9},
	  {9, 5, 8, 2, 4, 7, 3, 5, 1},
	  {7, 6, 2, 3, 9, 1, 4, 5, 8},
	  {3, 0, 1, 9, 5, 6, 8, 4, 2},
	  {4, 9, 6, 1, 8, 2, 5, 7, 3},
	  {2, 8, 5, 4, 7, 3, 9, 5, 6} };

    printf("Sequential:\n");
    checkSudoku_seq(*sudoku_valid);
    checkSudoku_seq(*sudoku_invalid);
    checkSudoku_seq(*sudoku_invalid_range);
    printf("Processes:\n");
    checkSudoku_proc(*sudoku_valid);
    checkSudoku_proc(*sudoku_invalid);
    checkSudoku_proc(*sudoku_invalid_range);
    printf("PThreads:\n");
    checkSudoku_pthreads(*sudoku_valid);
    checkSudoku_pthreads(*sudoku_invalid);
    checkSudoku_pthreads(*sudoku_invalid_range);
    printf("OpenMP:\n");
    checkSudoku_openmp(*sudoku_valid);
    checkSudoku_openmp(*sudoku_invalid);
    checkSudoku_openmp(*sudoku_invalid_range);
    return 0;
}
