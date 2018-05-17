#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main_aux.h"
#include "parser.h"
#include "game.h"
#include "solver.h"
#include <time.h>

void initializeBoard(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS]) {
	/*
	 * Initializes the sudoku board cells with default values: number - 0, isFixed - false
	 */
	int i,j;
	for (i=0; i < 10; i++){
	   for (j=0; j < 10; j++){
		   board[i][j].isFixed = false;
		   board[i][j].number = 0;
	   }
	}
}

void printSeperator() {
	/*
	 * Helper function to printBoard() which prints block seperators
	 */
	printf("----------------------------------\n");
}

void printBoard(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS]) {
	/*
	 * Prints the sudoku board according to the format
	 */
	int i,j;
	printSeperator();
	for (i=0; i < NUM_OF_ROWS; i++) {
		if (i == 3 || i == 6){
	    	printSeperator();
		}
		printf("| ");
		for (j=0; j < NUM_OF_COLUMNS; j++) {
			if (j == 3 || j == 6) {
				printf("| ");
			}
			if (board[i][j].isFixed) {
				printf(".");
				printf("%d", board[i][j].number);
			}
			else if (board[i][j].number != UNASSIGNED){
				printf(" ");
				printf("%d", board[i][j].number);
			}
			else{
				printf("  ");
			}
			printf(" ");
		}
		printf("|\n");
	}
	printSeperator();
}

void executeCommand(char *parsedCommand[4], cell user_board[NUM_OF_ROWS][NUM_OF_COLUMNS],cell solved_board[NUM_OF_ROWS][NUM_OF_COLUMNS], char* command){
	/*
	 * Evaluates game command (SET/HINT/VALIDATE/RESTART/EXIT) and calls the relavent function to execute it
	 */

	if (false) { /* DUMMY USE OF BOARD FOR COMPLIATION - REMOVE AFTER IMPLEMENTING COMMANDS */
		printf("%d", user_board[0][0].number);
	}

	if (strcmp(parsedCommand[0], "set") == 0) {
		set(user_board, atoi(parsedCommand[1]),atoi(parsedCommand[2]), atoi(parsedCommand[3]));
	} else if (strcmp(parsedCommand[0], "hint") == 0) {
		hint(solved_board, atoi(parsedCommand[1]), atoi(parsedCommand[2]));
	} else if (strcmp(parsedCommand[0], "validate") == 0) {
		validate(user_board, solved_board);
	} else if (strcmp(parsedCommand[0], "restart") == 0) {
		restart();
	} else if (strcmp(parsedCommand[0], "exit") == 0) {
		exitGame(command);
	} else { /* Invalid command */
		printf("Error: invalid command\n");
	}
}

bool valInBlock(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS], int  column, int row, int val){
	/*
	 * Checks if value exist in the block containing given row and column
	 */
    int initial_col, initial_row, col;
    initial_col = column - (column % BLOCK_SIZE);
    initial_row = row - (row % BLOCK_SIZE);
    for (col = initial_col; (col < BLOCK_SIZE + initial_col); col++) {
        for (row = initial_row; (row < BLOCK_SIZE + initial_row); row++) {
            if (board[row][col].number == val) {
                return true;
            }
        }
    }
    return false;
}

bool valInRow(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS], int row, int val){
	/*
	 * Checks if value exist in the given row
	 */
    int col;
    for (col = 0; col < NUM_OF_COLUMNS; col++) {
        if (board[row][col].number == val) {
            return true;
        }
    }
    return false;
}

bool valInColumn(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS], int column, int val) {
	/*
	 * Checks if value exist in the given column
	 */
    int row;
    for (row = 0; row < NUM_OF_ROWS; row++) {
        if (board[row][column].number == val) {
            return true;
        }
    }
    return false;
}

bool validCheck(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS], int column, int row, int val){
	/*
	 * Checks if validation of given value in cell <row,column> according to sudoku rules
	 */
    return !valInBlock(board, column, row, val) && !valInRow(board, row, val) && !valInColumn(board, column, val);
}

void gameOver(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS]){
	/*
	 * Checks if board is full
	 */
    int col, row;
    bool over = true;
    for (col = 0; col < NUM_OF_COLUMNS; col++) {
        for (row = 0; row < NUM_OF_ROWS; row++) {
            if (board[row][col].number == UNASSIGNED) {
                over = false;
            }
        }
    }
    if (over){
        printf(GAME_OVER);
    }
}

void set(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS], int column, int row, int val)	{
		/*
		 * Sets the value val to cell <row,column> on the sudoku board if its following the game rules
		 */
	if (val == 0){
		board[row - 1][column - 1].number = UNASSIGNED;
		printBoard(board);
	} else if (board[row - 1][column - 1].isFixed) {
		printf(FIXED_ERROR);
	} else if (validCheck(board, column - 1, row - 1, val)){
			board[row - 1][column - 1].number = val;
			printBoard(board);
	} else{
			printf(INVALID_ERROR);
		}
	gameOver(board);
}

void validate(cell user_board[NUM_OF_ROWS][NUM_OF_COLUMNS], cell solved_board[NUM_OF_ROWS][NUM_OF_COLUMNS]){
	/*
	 * returns true if current board is solvable according to the deterministic backtracking and updates stored solution
	 */
	cell copy_of_user_board[NUM_OF_ROWS][NUM_OF_COLUMNS];
	bool solvable;
	copyBoard(user_board, copy_of_user_board);
	solvable = recursiveBacktrack(copy_of_user_board, 0, 0, false);
	if (solvable){
		printf("Validation passed: board is solvable\n");
		copyBoard(copy_of_user_board, solved_board);
	} else{
		printf("Validation failed: board is unsolvable\n");
	}

}

void hint(cell solved_board[NUM_OF_ROWS][NUM_OF_COLUMNS], int column, int row){
	/*
	 * Prints the value of the cell <row,column> on the last solved sudoku board
	 */
	int hint;
		hint = solved_board[row][column].number;
		printf("Hint: set cell to %d\n", hint);
}

void restart(){
	/*
	 * Restarting the game
	 */
	cell solved_board[NUM_OF_ROWS][NUM_OF_COLUMNS];
	cell user_board[NUM_OF_ROWS][NUM_OF_COLUMNS];
	int fixedCells, error;
	char* command = malloc(MAX_CMD_SIZE);
	char *parsedCommand[4];
	setvbuf(stdout,NULL,_IONBF,0);
	initializeBoard(solved_board);
	initializeBoard(user_board);
	while (true) {
		printf("Please enter the number of cells to fill [0-80]:\n");
		error = scanf("%d", &fixedCells);
		if (error != 1){
			memoryError("main"); /* NEEDS TO BE CHECKED */
		}
		getchar(); /*Required cause of scanf-printf-fgets issue*/
		if (inputValid(fixedCells)){
			break;
		} else {
			printf("Error: Invalid number of cells to fill (should be between 0 and 80)\n");
		}
	}
	srand(fixedCells); /* NEEDS TO BE CHANGED WITH SEED */
	generateSolvedBoard(solved_board, fixedCells);
	generateUserBoard(solved_board, user_board);
	while (fgets(command, MAX_CMD_SIZE, stdin) != NULL) {
		parseCommand(command, parsedCommand);
		executeCommand(parsedCommand, user_board, solved_board, command);
	}
	if (feof(stdin)) { /* EOF */
		exitGame(command);
	} else if (ferror(stdin)) { /* Error */
		memoryError("main");
	}
}





