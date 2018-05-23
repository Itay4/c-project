#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "main_aux.h"
#include "game.h"

bool isValid(int number, cell board[NUM_OF_ROWS][NUM_OF_COLUMNS], int row, int column) {
    /*
     * Checks if given number is valid to set in given game board and position according to sudoku game rules
     */

    int i = 0;
    int sectorRow = 3 * (row / 3);
    int sectorCol = 3 * (column / 3);
    int row1 = (row + 2) % 3;
    int row2 = (row + 4) % 3;
    int col1 = (column + 2) % 3;
    int col2 = (column + 4) % 3;
 
    /* Check for the value in the given row and column */
    for (i = 0; i < 9; i++) {
        if (board[i][column].number == number) return false;
        if (board[row][i].number == number) return false;
    }
 
    /* Check the remaining four spaces in this sector */
    if (board[row1+sectorRow][col1+sectorCol].number == number) return false;
    if (board[row2+sectorRow][col1+sectorCol].number == number) return false;
    if (board[row1+sectorRow][col2+sectorCol].number == number) return false;
    if (board[row2+sectorRow][col2+sectorCol].number == number) return false;
    return true;
}

bool recursiveBacktrack(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS], int row, int column) {
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
            if (recursiveBacktrack(board, row+1, 0)) return true;
        } else {
            if (recursiveBacktrack(board, row, column+1)) return true;
        }
        return false;
    }
    for (i = 1; i < 10; i++){
        if (isValid(i, board, row, column)) {
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
        delFromArr(randomIndex + 1, j, availableNumbers);   
        j--;
        if (column == 8) {
            if (recursiveBacktrack(board, row + 1, 0)) return true;
        } else {
            if (recursiveBacktrack(board, row, column + 1)) return true;
        }
        /* Failed to find a valid value */
        board[row][column].number = 0;
        
	}
    return false;
}

bool isSafe(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS], int number, int row, int column) {
    /*
     * Check if safe to insert number in row and column in board according to sudoku game rules
     */
    int i, j, r, c;
    for(i = 0; i < 9; i++) {
        if(board[row][i].number == number) {
            return false;
        }
    }
    for(i = 0; i < 9; i++) {
        if(board[i][column].number == number) {
            return false;
        }
    }
    r = row - row % 3;
    c = column - column % 3;
    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            if(board[r + i][c + j].number == number) {
                return false;
            }
        }
    }
    return true;
}
bool deterministicBacktrack(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS]) {
    /*
     * Deterministic backtrack to check if sudoku board is valid
     */
    bool flag = false;
    int i, j, row, column;
    for(i = 0; i < 9; i++) {
        for(j = 0; j < 9; j++) {
            if(board[i][j].number == 0) {
                flag=true;
                break;
            }
        }
        if(flag) {
            break;
        }
    }
    row = i;
    column = j;
    if(!flag) {
        return true;
    } else {
        for(i = 1; i <= 9; i++) {
            if(isSafe(board, i, row, column)) {
                board[row][column].number = i;
                if(deterministicBacktrack(board)) {
                    return true;
                }
                board[row][column].number = 0;
            }
        }
    }
    return false;
}

void setFixedCells(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS], int fixedCells) {
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

void generateSolvedBoard(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS], int fixedCells){
    /*
     * Generates solved sudoku game board
     */
    recursiveBacktrack(board, 0, 0);
    setFixedCells(board, fixedCells);
}

void generateUserBoard(cell solved_board[NUM_OF_ROWS][NUM_OF_COLUMNS], cell user_board[NUM_OF_ROWS][NUM_OF_COLUMNS]) {
    /*
     * Generates unsolved user sudoku game board
     */
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
}

