#include <time.h>

#ifndef SOLVER_H
#define SOLVER_H

int recursiveBacktrack(cell **board, int row, int column, bool rand);
void setFixedCells(cell **board, int fixedCells);
void generateSolvedBoard(cell **board, int fixedCells);
void generateUserBoard(cell **board, cell user_board[NUM_OF_ROWS][NUM_OF_COLUMNS]);
cell** duplicate_board(cell** oldBoard);
int ILP(cell **b, cell **solvedBoard);
int deterministic_backtrack(cell** board, int i, int j);

#endif /* SOLVER_H */
