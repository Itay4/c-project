#include <time.h>

#ifndef SOLVER_H
#define SOLVER_H

int recursiveBacktrack(cell **board, int row, int column, bool rand);
void setFixedCells(cell **board, int fixedCells);
void generateSolvedBoard(cell **board, int fixedCells);
void generateUserBoard(cell **board, cell user_board[NUM_OF_ROWS][NUM_OF_COLUMNS]);
void ILP(cell **board, int rows, int cols);
int deterministic_backtrack(cell** board, int i, int j);

#endif /* SOLVER_H */
