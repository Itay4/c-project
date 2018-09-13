#include <time.h>

#ifndef SUDOKU90_SOLVER_H
#define SUDOKU90_SOLVER_H

cell** duplicate_board(cell** oldBoard);
int ILP(cell **b, cell **solvedBoard);
int deterministic_backtrack(cell** board, int i, int j);
void copy_random_cells(cell **solvedBoard, int copyCells, cell** finalBoard);

#endif /*SUDOKU90_SOLVER_H*/
