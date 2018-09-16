/** @file solver.h
 *  @brief solver header file.
 *
 *  This header contains the prototypes for the solver module.
 *
 *  @author Itay Keren (itaykeren)
 *  @author Rotem Bar (rotembar)
 *
 */
#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H


/* -- Includes -- */

#include "main_aux.h"
#include <time.h>


/**
 *  @brief Duplicates sudoku board.
 *  @param oldBoard - board to duplicate.
 *  @return Duplicated sudoku board.
 */
cell** duplicate_board(cell** oldBoard);


/**
 *  @brief Checks if sudoku board is feasible using ILP.
 *  @param b - Board to validate.
 *  @param solvedBoard - Copy of board.
 *  @return 1 if feasible, else 0.
 */
int ILP(cell **b, cell **solvedBoard);


/**
 *  @brief Counts number of solutions to sudoku board using deterministic backtrack.
 *  @param board - Board to count solutions to.
 *  @param i - Rows index.
 *  @param j - Columns index.
 *  @return Number of solutions.
 */
int deterministic_backtrack(cell** board, int i, int j);


/**
 *  @brief Randomly chooses given number of cells and unassign them.
 *  @param solvedBoard - Board to assign values to.
 *  @param copyCells - Number of cells.
 *  @param finalBoard - Board which will contain the assigned valued at the end of the function.
 *  @return void.
 */
void copy_random_cells(cell **solvedBoard, int copyCells, cell** finalBoard);

#endif /*SUDOKU_SOLVER_H*/
