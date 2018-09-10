
#ifndef SUDOKU90_SOLVER_H
#define SUDOKU90_SOLVER_H

#include <time.h>


bool isValid(int number, cell board[NUM_OF_ROWS][NUM_OF_COLUMNS], int row, int column);
int recursiveBacktrack(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS], int row, int column, bool rand);
void setFixedCells(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS], int fixedCells);
void generateSolvedBoard(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS], int fixedCells);
void generateUserBoard(cell solved_board[NUM_OF_ROWS][NUM_OF_COLUMNS], cell user_board[NUM_OF_ROWS][NUM_OF_COLUMNS]);
void ILP(cell **board, int rows, int cols);


#endif /*SUDOKU90_SOLVER_H*/
