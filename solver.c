

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "main_aux.h"
#include "game.h"




bool recursive_backtrack(cell** board, int row, int column) {
    /*
     * Recursive backtrack to solve sudoku board
     */
    int nextNum, i, j;
    int randomIndex;
    int availableNumbers[9];
    j = 0;
    if (row == 9) {
        return true;
    }

    /* If cell number is already set, no need to change and recruse to next cell */
    if (board[row][column].number) {
        if (column == 8) {
            if (recursive_backtrack(board, row+1, 0)) return true;
        } else {
            if (recursive_backtrack(board, row, column+1)) return true;
        }
        return false;
    }
    for (i = 1; i < 10; i++){
        if (valid_check(board, column, row, i)) {
            availableNumbers[j] = i;
            j++;}
    }
    while (j > 0) {
        if (j==1) { /* Only a single legal value remains.  */
            nextNum = availableNumbers[0];
        } else {
            randomIndex = rand() % j;
            nextNum = availableNumbers[randomIndex];
        }
        board[row][column].number = nextNum;
        del_from_arr(randomIndex + 1, j, availableNumbers);
        j--;
        if (column == 8) {
            if (recursive_backtrack(board, row + 1, 0)) return true;
        } else {
            if (recursive_backtrack(board, row, column + 1)) return true;
        }
        /* Failed to find a valid value */
        board[row][column].number = 0;

    }
    return false;
}



void set_fixed_cells(cell **board, int fixedCells) {
    /*
     * Randomly chooses given number of cells in the game board and sets them as fixed
     */
    int i, randX, randY;
    for (i = 0; i < fixedCells; i++){
        while (true) {
            randX = rand() % NUM_OF_ROWS;
            randY = rand() % NUM_OF_COLUMNS;
            if (board[randY][randX].isFixed != true){
                board[randY][randX].isFixed = true;
                break;
            }
        }

    }
}

void generate_solved_board(cell** board, int fixedCells){
    /*
     * Generates solved sudoku game board
     */
    recursive_backtrack(board, 0, 0);
    set_fixed_cells(board, fixedCells);
}

/*void generate_user_board(cell **solved_board, cell** user_board) {

     * Generates unsolved user sudoku game board

	int i,j;
	for (i=0; i < NUM_OF_ROWS; i++) {
		for (j=0; j < NUM_OF_COLUMNS; j++) {
			if (solved_board[i][j].isFixed) {
				user_board[i][j].number = solved_board[i][j].number;
				user_board[i][j].isFixed = true;
			}
			else{
				user_board[i][j].number = UNASSIGNED;
				user_board[i][j].isFixed = false;
			}
		}
	}
	printBoard(user_board);
}*/






