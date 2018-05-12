#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void parseCommand(char* command, char* parsedCommand[]) {
	/*
	 * Parses string by white spaces and assigns result to an array
	 */

	char *token;
	char *delimiter = " \t\r\n";
	int i = 0;
	token = strtok(command, delimiter);
	while (token != NULL && i < 4) {
		parsedCommand[i] = token;
		i++;
		token = strtok(NULL, delimiter);
	}
}


