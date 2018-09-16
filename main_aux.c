/** @file main_aux.c
 *  @brief main_aux source file.
 *
 *  This module contains auxiliary functions to be used in other modules.
 *
 *  @author Itay Keren (itaykeren)
 *  @author Rotem Bar (rotembar)
 *
 */

/* -- Includes -- */
#include "main_aux.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

/* -- Global Variables  -- */
extern int blockRows;
extern int blockCols;
extern int markErrors;
extern char mode;
extern bool gameOver;


/* Simulates deletion of an element in given position in an int pointer. */
void delFromArr(int position, int numbersLeft, int* availableNumbers) {
    int i = 0;
    int j;
    int* tmpArray = malloc(numbersLeft * sizeof(int));
    if (tmpArray == NULL) {
        memory_error("Memory allocation failed\n");
        exit(0);
    }
    for ( j = 0; j < numbersLeft + 1; j++ ){
        if (j != position) {
            tmpArray[i] = availableNumbers[j];
            i++;
        }
    }
    for ( j = 0; j < numbersLeft; j++ ){
        availableNumbers[j] = tmpArray[j];
    }
    free(tmpArray);
}

/* Prints error message */
void memory_error(char* func){
    printf("Error: %s has failed\n", func);
}

/* returns true if s can be converted to int, else returns false */
bool is_integer(char* s){
    char * t;
    for (t = s; *t != '\0'; t++) {
        if (*t == '.' || isalpha(*t)) {
            return false;
        }
    }
    return true;
}

/* returns an int representing corresponding starting block column index that matches given column */
int get_block_col_index(int column){
    int initialCol;
    int blockNumberCols = 1 + ((column - 1) / blockCols);
    initialCol = blockCols * (blockNumberCols - 1);
    return initialCol;
}

/* returns an int representing corresponding starting block row index that matches given row */
int get_block_row_index(int row){
    int initialRow;
    int blockNumberRows = 1 + ((row - 1) / blockRows);
    initialRow = blockRows * (blockNumberRows - 1);
    return initialRow;
}

/* returns true if index is between 1-N (including), else returns false */
bool valid_board_index(int index, int N){
    if (index < 1 || index > N ) {
        return false;
    }
    return true;
}

/* Returns true if index is between 0-N (including), else returns false */
bool valid_set_value(int val, int N){
    if (val < 0 || val > N ) {
        return false;
    }
    return true;
}

/* Returns an array representing next empty unassigned cell position */
int* get_next_play(cell** board) {
    int i, j, N;
    int* auxArray = (int*) calloc(2, sizeof(int));
    if (auxArray == NULL) {
        memory_error("Memory allocation failed\n");
        exit(0);
    }
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

/* generates an int pointer to represent an int array containg values from 1-maxVal */
int* generate_int_array(int maxVal) {
    int* array;
    int i;
    array = malloc(maxVal * sizeof(int));
    if (array == NULL) {
        memory_error("Memory allocation failed\n");
        exit(0);
    }
    for (i = 1; i < maxVal + 1; i++){
        array[i-1] = i;
    }
    return array;
}