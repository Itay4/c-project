#include "parser.h"
#include "main_aux.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#define MAX_CMD_SIZE 10
#define NUM_OF_LINES 9
#define NUM_OF_ROWS 9

/*typedef int bool;
#define true 1
#define false 0*/
int main() {
	
	cell (*board) [NUM_OF_ROWS] = malloc(sizeof(cell[NUM_OF_LINES][NUM_OF_ROWS]));
	int fixedCells;
	char* command = malloc(MAX_CMD_SIZE);
	char *parsedCommand[4];
	int i,j; /*REMOVE*/
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
	/*REMOVE THIS*/
	for (i=0; i < 10; i++){
	   for (j=0; j < 10; j++){
	   	if (j%2 == 0){
		   board[i][j].isFixed = false;
		} else {
			board[i][j].isFixed = true;
		}
	      board[i][j].number = 0;
	   }
	}
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


