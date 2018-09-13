#include <time.h>

#ifndef SOLVER_H
#define SOLVER_H

cell** duplicate_board(cell** oldBoard);
int ILP(cell **b, cell **solvedBoard);
int deterministic_backtrack(cell** board, int i, int j);
int recursive_backtrack(cell **board, int row, int column, bool rand);
void set_fixed_cells(cell **board, int fixedCells);
void generate_solved_board(cell **board, int fixedCells);
void generate_user_soard(cell **board, cell **user_board);

#endif /* SOLVER_H */
