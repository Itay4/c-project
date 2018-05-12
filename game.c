#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main_aux.h"

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
			} else {
				printf("  ");
			}
			printf(" ");
		}
		printf("|\n");
		
	}
	printSeperator();
}

void executeCommand(char *parsedCommand[4], cell board[NUM_OF_ROWS][NUM_OF_COLUMNS], char* command) {
	/*
	 * Evaluates game command (SET/HINT/VALIDATE/RESTART/EXIT) and calls the relavent function to execute it
	 */

	if (false) { /* DUMMY USE OF BOARD FOR COMPLIATION - REMOVE AFTER IMPLEMENTING COMMANDS */
		printf("%d", board[0][0].number);
	}

	if (strcmp(parsedCommand[0], "set") == 0) {
		/* TODO: set */
	} else if (strcmp(parsedCommand[0], "hint") == 0) {
		/* TODO: hint */
	} else if (strcmp(parsedCommand[0], "validate") == 0) {
		/* TODO: validate */
	} else if (strcmp(parsedCommand[0], "restart") == 0) {
		/* TODO: restart */
	} else if (strcmp(parsedCommand[0], "exit") == 0) {
		exitGame(command);
		/* TODO: exit */
	} else { /* Invalid command */
		printf("Error: invalid command\n");
	}
}