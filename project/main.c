#include "main_aux.h"
#include "parser.h"
#include "game.h"
#include "solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {

	char *parsedCommand[4] = {'\0', '\0', '\0', '\0'};
	char *command = malloc(MAX_CMD_SIZE + 1);
	size_t rows, cols;
	cell **board;
	int argsCounter;
	char mode = 'I';
	int i;
	int markErrors = 1;

	printf("Sudoku\n------\n");
	printf("Enter your command:\n");
		
	while (fgets(command, MAX_CMD_SIZE, stdin) != NULL) {

		argsCounter = parseCommand(command, parsedCommand);
		if (parsedCommand[0] == NULL)  {
			/*Handles blank line*/
		}
		 else if (strcmp(parsedCommand[0], "solve") == 0) {
			board = NULL;
			board = solveCommand(parsedCommand, &rows, &cols, &markErrors);
			mode = 'S';
		} else if (strcmp(parsedCommand[0], "edit") == 0){
			board = NULL;
			board = editCommand(parsedCommand, &rows, &cols);
			mode = 'E';
		} else {	
			executeCommand(parsedCommand, board, command, argsCounter, mode, &rows, &cols, &markErrors);
		}
		for (i = 0; i < 4; i++){
			parsedCommand[i] = '\0';
		}
		printf("Enter your command:\n");
	}

	return 0;
}


