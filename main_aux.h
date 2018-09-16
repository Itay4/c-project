/** @file main_aux.h
 *  @brief main_aux header file.
 *
 *  This header contains the prototypes for the main_aux module.
 *  contains defines used in program flow.
 *  contains the structure defenition of a sudoku board cell.
 *
 *  @author Itay Keren (itaykeren)
 *  @author Rotem Bar (rotembar)
 *
 */

#ifndef SUDOKU_MAIN_AUX_H
#define SUDOKU_MAIN_AUX_H

/* Defines */
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
#define ERRONEOUS_ERROR "Error: board contains erroneous values\n"

/* -- Structs -- */

/**
 * @brief cell structure representing a sudoku board cell.
 * cell contains number field containing cell value.
 * cell contains isFixed field containing true if cell is fixed, else false.
 * cell contains asterisk field containing true if cell value is invalid, else false.
 *
 */
typedef struct  {
    int number;
    bool isFixed;
    bool asterisk;
} cell;

/* -- Global Variables  -- */
extern int blockRows;
extern int blockCols;
extern int markErrors;
extern char mode;
extern bool gameOver;


/** @brief Simulates deletion of an element in given position in an int pointer.
 *  @param position index of position to delete.
 *  @param numbersLeft amount of items to be left after deletion
 *  @param availableNumbers int pointer to be updated.
 *  @return void.
 */
void delFromArr(int position, int numbersLeft, int* availableNumbers);


/** @brief Prints error message in given func.
 *  @param func strring representing location of error.
 *  @return void.
 */
void memory_error(char* func);


/** @brief checks if s can be converted to int, else returns false
 *  @param s string representing a potential int.
 *  @return true if s can be converted to an int, else false.
 */
bool is_integer(char* s);


/** @brief computes index of corresponding starting block column index that matches given column
 *  @param column index.
 *  @return int that matches starting column block index.
 */
int get_block_col_index(int column);


/** @brief computes index of corresponding starting block row index that matches given row
 *  @param row index.
 *  @return int that matches starting row block index.
 */
int get_block_row_index(int row);


/** @brief checks if index is valid board index
 *  between 1-N (including)
 *  @param index to be checked.
 *  @param N
 *  @return true if in range, else false.
 */
bool valid_board_index(int index, int N);


/** @brief checks if index is valid set index
 *  between 0-N (including)
 *  @param index to be checked.
 *  @param N
 *  @return true if in range, else false.
 */
bool valid_set_value(int val, int N);


/** @brief Returns an array representing next empty unassigned cell position.
 *  @param board current game board.
 *  @return an array representing next empty unassigned cell position
 */
int* get_next_play(cell** board);


/** @brief Returns an int pointer containing numbers from 1-maxVal.
 *  @param maxVal
 *  @return int pointer.
 */
int* generate_int_array(int maxVal);


#endif /*SUDOKU_MAIN_AUX_H*/
