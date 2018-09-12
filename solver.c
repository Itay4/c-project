#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "main_aux.h"
#include "game.h"
#include "stack.h"
#include <string.h>
#define UNUSED(x) (void)(x)


typedef struct validPlays {
    int* validPlaysArray;
    int numOfPlays;
} validPlays;

bool recursiveBacktrack(cell** board, int row, int column) {
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
        if (validCheck(board, column, row, i)) {
            availableNumbers[j] = i;
            j++;}
    }
    while (j > 0) {
        if (j==1) { 
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
        board[row][column].number = 0;

    }
    return false;
}

int get_actual_value(cell** board, int i, int j) {
    /* Gets the actual value of cell */
    int value = board[i][j].number;
    int N = rows*cols;
    if (value < 0) {
        value = value * (-1);
    }
    if (value > N) { 
        value = value - N;
    }
    return value;
}

validPlays* get_valid_plays(cell** board, int i, int j) {

    int* plays;
    int* legalPlays;
    int counter, k, l, roundI, roundJ, value, N;
    validPlays* result;

    N = rows*cols;
    counter = 0;
    plays = calloc(N+1, sizeof(int));
    for (k = 0; k < N; k++) {
        value = get_actual_value(board, k, j);
        if (value != 0) {
            plays[value] = 1;
        }
    }
    for (k = 0; k < N; k++) {
        value = get_actual_value(board, i, k);
        if (value != 0) {
            plays[value] = 1;
        }
    }

    roundI = i/rows;
    roundJ = j/cols;
    for (k = rows*roundI; k < rows*(roundI)+rows; k++){
        for(l = cols*roundJ; l<cols+cols*roundJ; l++) {
            value = get_actual_value(board, k, l);
            if (value != 0) {
            plays[value] = 1;
            }
        }
    }
    for (k = 1; k < N+1; k++) {
        if (plays[k] == 0) {
            counter++;
        }
    }
    legalPlays = calloc(counter, sizeof(int));
    l = 0;
    for (k = 1; k < N+1; k++) {
        if (plays[k] == 0) {
            legalPlays[l] = k;
            l++;
        }
    }
    result = (validPlays*) malloc(sizeof(validPlays));
    result->validPlaysArray = legalPlays;
    result->numOfPlays = counter;
    free(plays);
    return result;
}

cell** duplicate_board(cell** oldBoard) {
    /* Duplicates sudoku board */
    cell** newBoard;
    int N, i, j;
    N = rows * cols;
    newBoard=generateEmptyBoard();
    for (i = 0; i < N; i++){
        for (j = 0; j < N; j++) {
            newBoard[i][j].number = oldBoard[i][j].number;
        }
    }
    return newBoard;
}

int deterministic_backtrack(cell** board, int i, int j) {
    /* Counts number of solutions using exhaustive deterministic backtrack */
    element* e;
    int counter, firstPlay, N, k, nextI, nextJ, newI, newJ;
    int data[2];
    int* auxData;

    validPlays* legalPlays;
    cell** auxBoard;
    cell** finalBoard;
    stack* stck=calloc(1,sizeof(stack));
    N = rows * cols;

    stack_initialize(stck);
    data[0]=i;
    data[1]=j;
    push(data, board, stck);

    while (stck->counter > 0){
        e = pop(stck);
        auxData = e->data;
        if (firstPlay == 0){
            firstPlay = 1;
            auxBoard = duplicate_board(board);
            nextI = data[0];
            nextJ = data[1];
            newI = data[0];
            newJ = data[1];
        } else {
            auxBoard = duplicate_board(e->board);
            nextI = auxData[0];
            nextJ = auxData[1];
            newI = auxData[0];
            newJ = auxData[1];
        }
        legalPlays = (validPlays*) get_valid_plays(auxBoard, newI, newJ);
        do{
            nextJ = (nextJ+1) % N;
            if (nextJ == 0){
                nextI = (nextI+1) % N;
            }
        }
        while((nextJ != N-1 || nextI != N-1) && auxBoard[nextI][nextJ].number != 0);

        if((((newI == N-1 && newJ == N-1)|| (nextI == N-1 && nextJ == N-1 && auxBoard[nextI][nextJ].number!=0)) && ((legalPlays->numOfPlays) == 1))) {
                counter++;
                continue;
            } else {
            for (k = 0; k < (legalPlays->numOfPlays); k++){
                auxData = calloc(2,sizeof(int));
                finalBoard = duplicate_board(auxBoard);
                finalBoard[newI][newJ].number = (legalPlays->validPlaysArray)[k];
                auxData[0] = nextI;
                auxData[1] = nextJ;
                push(auxData, finalBoard, stck);
            }

            free(legalPlays->validPlaysArray);
            free(legalPlays);
            }
        }
    free(stck);
    return counter;
}

void setFixedCells(cell **board, int fixedCells) {
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

void generateSolvedBoard(cell** board, int fixedCells){
    /*
     * Generates solved sudoku game board
     */
    recursiveBacktrack(board, 0, 0);
    setFixedCells(board, fixedCells);
}

/*void generateUserBoard(cell solved_board[NUM_OF_ROWS][NUM_OF_COLUMNS], cell user_board[NUM_OF_ROWS][NUM_OF_COLUMNS]) {

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