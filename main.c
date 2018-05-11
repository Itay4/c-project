#include "main_aux.h"
#include "parser.h"
#include "game.h"
#include "solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_CMD_SIZE 10
#define NUM_OF_ROWS 9
#define NUM_OF_COLUMNS 9

int main() {
	
	cell board[NUM_OF_ROWS][NUM_OF_COLUMNS];
	int fixedCells;
	char* command = malloc(MAX_CMD_SIZE);
	char *parsedCommand[4];
	initializeBoard(board);
	while (true) {
		printf("Please enter the number of cells to fill [0-80]:\n");
		scanf("%d", &fixedCells);
		getchar(); /*Required cause of scanf-printf-fgets issue*/
		if (inputValid(fixedCells)){
			break;
		} else {
			printf("Error: Invalid number of cells to fill (should be between 0 and 80)\n");
		}
	}
	srand(fixedCells);
	generateBoard(board, fixedCells);
	printBoard(board); 
	fgets(command, MAX_CMD_SIZE, stdin);
	parseCommand(command, parsedCommand);
	
	if (strcmp(parsedCommand[0], "set") == 0) {
	} else if (strcmp(parsedCommand[0], "hint") == 0) {
	} else if (strcmp(parsedCommand[0], "validate") == 0) {
	} else if (strcmp(parsedCommand[0], "restart") == 0) {
	} else if (strcmp(parsedCommand[0], "exit") == 0) {
	} else { 
		printf("Error: invalid command\n");
	}

	return 0;
}


