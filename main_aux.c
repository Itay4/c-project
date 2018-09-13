#include "main_aux.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

extern int blockRows;
extern int blockCols;
extern int mark_errors;

void delFromArr(int position, int numbersLeft, int* availableNumbers) {
    /*
     * Simulates deletion of elemnt in given position in array by shifting left elements
     */
    int i;
    for ( i = position - 1 ; i < numbersLeft - 1 ; i++ ) {
        availableNumbers[i] = availableNumbers[i+1];
    }
}


void memory_error(char* func){
    /*
     * Prints error message and exists the program
     */
    printf("Error: %s has failed\n", func);
    exit(0);
}

bool is_integer(char *s){
    char * t;
    for (t = s; *t != '\0'; t++) {
        if (*t == '.' || isalpha(*t)) {
            return false;
        }
    }
    return true;
}

int get_block_col_index(int column){
    int initialCol;
    int blockNumberCols = 1 + ((column - 1) / blockCols);
    initialCol = blockCols * (blockNumberCols - 1);
    return initialCol;
}

int get_block_row_index(int row){
    int initialRow;
    int blockNumberRows = 1 + ((row - 1) / blockRows);
    initialRow = blockRows * (blockNumberRows - 1);
    return initialRow;
}
bool valid_board_index(int index, int N){
    if (index < 1 || index > N ) {
        return false;
    }
    return true;
}

bool valid_set_value(int val, int N){
    if (val < 0 || val > N ) {
        return false;
    }
    return true;
}

int* get_next_play(cell** board) {
    /* Returns an array representing next empty unassigned cell position */
    int* auxArray = (int*) calloc(2, sizeof(int));
    int i, j, N;
    auxArray[0] = -1;
    auxArray[1] = -1;
    N = blockRows * blockCols;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (board[i][j].number == 0) {
                auxArray[0] = i;
                auxArray[1] = j;
                return auxArray;
            }
        }
    }
    return auxArray;
}

int * generate_int_array(int maxVal)
{
    int* array;
    int i;
    array = malloc(maxVal * sizeof(int));
    for (i = 1; i < maxVal + 1; i++){
       array[i-1] = i;
    }
return array;
}

