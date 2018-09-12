
#ifndef SUDOKU90_SOLVER_H
#define SUDOKU90_SOLVER_H

#include <time.h>



int recursive_backtrack(cell **board, int row, int column, bool rand);
void set_fixed_cells(cell **board, int fixedCells);
void generate_solved_board(cell **board, int fixedCells);
void generate_user_soard(cell **board, cell **user_board);
void ILP(cell **board, int rows, int cols);


#endif /*SUDOKU90_SOLVER_H*/
