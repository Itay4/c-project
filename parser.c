/** @file parser.c
 *  @brief parser source file.
 *
 *  This module is used to parser user input to be later executed as game commands.
 *
 *  @author Itay Keren (itaykeren)
 *  @author Rotem Bar (rotembar)
 *
 */

#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Parses string by white spaces and assigns result to an array */
int parseCommand(char* command, char* parsedCommand[]) {
    char *token;
    char *delimiter = " \t\r\n";
    int i = 0;
    token = strtok(command, delimiter);
    while ((token != NULL) && (i < 4)) {
        parsedCommand[i] = token;
        i++;
        token = strtok(NULL, delimiter);
    }
    return i;
}
