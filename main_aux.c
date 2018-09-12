
#include "main_aux.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

bool inputValid(int num) {
    /*
     * Validates user input of number of cells to fill
     */

    if (num >= 0 && num <= 80){
        return true;
    }
    return false;
}

void delFromArr(int position, int numbersLeft, int availableNumbers[]) {
    /*
     * Simulates deletion of element in given position in array by shifting left elements
     */
    int c;
    for ( c = position - 1 ; c < numbersLeft - 1 ; c++ ) {
        availableNumbers[c] = availableNumbers[c+1];
    }
}

void exitGame(char* command){
    /*
     * Free up memeory and exists the program
     */
    printf("Exiting...\n");
    free(command);

    exit(0);
}

void memoryError(char* func){
    /*
     * Prints error message and exists the program
     */
    printf("Error: %s has failed\n", func);
    exit(0);
}

void copyBoard(cell **source_board, cell **new_board){
    /*
     * Copying source sudoku board to new sudoku board.
     */
    int i,j;
    int N = rows * cols;

    for (i=0; i < N; i++) {
        for (j=0; j < N; j++){
            /*printf("I: %d, J: %d\n", i, j);*/

            new_board[i][j].number = source_board[i][j].number;
            new_board[i][j].asterisk = source_board[i][j].asterisk;
            new_board[i][j].isFixed = source_board[i][j].isFixed;

        }
    }
}

bool isInteger(char *s){
    char * t;
    for (t = s; *t != '\0'; t++) {
        if (*t == '.' || isalpha(*t)) {
            return false;
        }
    }
    return true;
}

void freeBoard(cell** board){
    int i;
    int N = rows * cols;
    for (i = 0; i < N; i++) {
        free(board[i]);
    }
    free(board);
    board = NULL;

}