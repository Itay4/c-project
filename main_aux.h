
#ifndef SUDOKU90_MAIN_AUX_H
#define SUDOKU90_MAIN_AUX_H

#include <ctype.h>


typedef int bool;
#define true 1
#define false 0
#define NUM_OF_ROWS 9
#define NUM_OF_COLUMNS 9
/* Constants */
#define MAX_CMD_SIZE 1024 /*check if not 256. size of char is 1*/
extern int rows;
extern int cols;
extern int mark_errors;
#define BLOCK_SIZE 3
#define UNASSIGNED 0
#define FIXED_ERROR "Error: cell is fixed\n"
#define INVALID_ERROR "Error: value is invalid\n"
#define GAME_OVER "Puzzle solved successfully\n"
#define VALIDATION_ERROR "Validation failed: board is unsolvable\n"

/* Types */
typedef struct  {
    int number;
    bool isFixed;
    bool asterisk;
} cell;

bool inputValid(int num);

void delFromArr(int position, int numbersLeft, int availableNumbers[]);

void exitGame(char* command);

void memoryError(char* func);

void copyBoard(cell **source_board, cell **new_board);

bool isInteger(char *s);

void freeBoard(cell** board);

#endif /*SUDOKU90_MAIN_AUX_H*/
