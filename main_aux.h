#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

typedef int bool;
#define true 1
#define false 0

/* Constants */
#define MAX_CMD_SIZE 1024
#define NUM_OF_ROWS 9
#define NUM_OF_COLUMNS 9
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
} cell;

bool inputValid(int num);

void delFromArr(int position, int numbersLeft, int availableNumbers[]);

void exitGame(char* command);

void memoryError(char* func);

void copyBoard(cell source_board[NUM_OF_ROWS][NUM_OF_COLUMNS], cell new_board[NUM_OF_ROWS][NUM_OF_COLUMNS]);

#endif /* MAIN_AUX_H_ */

