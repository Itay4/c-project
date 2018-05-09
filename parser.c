#include <string.h>
#include <stdio.h>
#include <stdlib.h>
void parseCommand(char* command, char* parsedCommand[]) {
	char *token;
	char *delimiter = " \t\r\n";
	int i = 0;
	token = strtok(command, delimiter);
	while (token != NULL) {
		parsedCommand[i] = token;
		i++;
		token = strtok(NULL, delimiter);
	}
}


