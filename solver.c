#include <stdio.h>
#include <time.h>
#include <stdlib.h>
typedef int bool;
#define true 1
#define false 0
#define NUM_OF_ROWS 9
#define NUM_OF_COLUMNS 9

typedef struct  {
	int number;
	bool isFixed;
} cell;

bool isValid(int number, cell board[NUM_OF_ROWS][NUM_OF_COLUMNS], int row, int column) {
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

void delFromArr(int position, int numbersLeft, int availableNumbers[]) {
    int c;
    for ( c = position - 1 ; c < numbersLeft - 1 ; c++ ) {
         availableNumbers[c] = availableNumbers[c+1];
     }
}

bool recursiveBacktrack(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS], int row, int column) {
	
    int nextNum;
    int randomIndex;
    int availableNumbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9}; 
    int numbersLeft = 9;
    
    

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
    while (numbersLeft > 0) {
        randomIndex = rand() % numbersLeft;
        nextNum = availableNumbers[randomIndex];
        numbersLeft--;
        if(isValid(nextNum, board, row, column)) {
            board[row][column].number = nextNum;
            delFromArr(randomIndex, numbersLeft, availableNumbers);
            if (column == 8) {
                if (recursiveBacktrack(board, row + 1, 0)) return true;
            } else {
                if (recursiveBacktrack(board, row, column + 1)) return true;
            }
            /* Failed to find a valid value */
            board[row][column].number = 0;
        }
	}
    return false;
}

void setFixedCells(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS], int fixedCells){
    int i, randX, randY;
    for (i = 0; i < fixedCells; i++){
        while (true) {
            randX = rand() % NUM_OF_ROWS;
            randY = rand() % NUM_OF_COLUMNS;
            if (board[randX][randY].isFixed != true){
                board[randX][randY].isFixed = true;
                break;
            }
        }

    }
}

void generateBoard(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS], int fixedCells){
    recursiveBacktrack(board, 0, 0);
    setFixedCells(board, fixedCells);
}

