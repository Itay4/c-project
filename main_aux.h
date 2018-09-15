
#ifndef SUDOKU90_MAIN_AUX_H
#define SUDOKU90_MAIN_AUX_H

extern int blockRows;
extern int blockCols;
extern int markErrors;

typedef int bool;
#define true 1
#define false 0

#define UNASSIGNED 0
#define MAX_CMD_SIZE 256

#define FIXED_ERROR "Error: cell is fixed\n"
#define VALUE_RANGE_ERROR "Error: value not in range 0-%d\n"
#define MARK_ERROR_ERROR "Error: the value should be 0 or 1\n"
#define INVALID_ERROR "ERROR: invalid command\n"
#define GAME_OVER "Puzzle solved successfully\n"
#define VALIDATION_ERROR "Validation failed: board is unsolvable\n"
#define ERRONEOUS_ERROR "Error: board contains erroneous values\n"

/* Types */
typedef struct  { /* TODO: sonsider to move to game.h */
    int number;
    bool isFixed;
    bool asterisk;
} cell;

void delFromArr(int position, int numbersLeft, int* availableNumbers);

void memory_error(char* func);

bool is_integer(char *s);

int get_block_col_index(int column);

int get_block_row_index(int row);

bool valid_board_index(int index, int N);

bool valid_set_value(int val, int N);

int* get_next_play(cell** board);

int * generate_int_array(int maxVal);

#endif /*SUDOKU90_MAIN_AUX_H*/
