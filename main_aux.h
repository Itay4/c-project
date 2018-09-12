
#ifndef SUDOKU90_MAIN_AUX_H
#define SUDOKU90_MAIN_AUX_H

extern int blockRows;
extern int blockCols;
extern int markErrors;

typedef int bool;
#define true 1
#define false 0

#define UNASSIGNED 0
#define NUM_OF_ROWS 9  /* TODO: remove */
#define NUM_OF_COLUMNS 9 /* TODO: remove */
#define BLOCK_SIZE 3 /* TODO: remove */
#define MAX_CMD_SIZE 1024 /* TODO:check if not 256. size of char is 1*/

#define FIXED_ERROR "Error: cell is fixed\n"
#define VALUE_RANGE_ERROR "Error: value not in range 0-%d\n"
#define MARK_ERROR_ERROR "Error: the value should be 0 or 1\n"
#define INVALID_ERROR "ERROR: invalid command\n"
#define GAME_OVER "Puzzle solved successfully\n"
#define VALIDATION_ERROR "Validation failed: board is unsolvable\n"

/* Types */
typedef struct  { /* TODO: sonsider to move to game.h */
    int number;
    bool isFixed;
    bool asterisk;
} cell;





void del_from_arr(int position, int numbersLeft, int availableNumbers[]);

void memory_error(char* func);

bool is_integer(char *s);

int get_block_col_index(int column);

int get_block_row_index(int row);

#endif /*SUDOKU90_MAIN_AUX_H*/
