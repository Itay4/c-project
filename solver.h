
#ifndef SUDOKU90_SOLVER_H
#define SUDOKU90_SOLVER_H

#include <time.h>



int recursiveBacktrack(cell **board, int row, int column, bool rand);
void setFixedCells(cell **board, int fixedCells);
void generateSolvedBoard(cell **board, int fixedCells);
void generateUserBoard(cell **board, cell user_board[NUM_OF_ROWS][NUM_OF_COLUMNS]);
void ILP(cell **board, int rows, int cols);


#endif /*SUDOKU90_SOLVER_H*/
