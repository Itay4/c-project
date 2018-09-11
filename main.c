#include "main_aux.h"
#include "parser.h"
#include "game.h"
#include "solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "linked_list.h"

int rows = 0;
int cols = 0;
int mark_errors = 1;

int main() {

    char *parsedCommand[4] = {'\0', '\0', '\0', '\0'};
    char *command = malloc(MAX_CMD_SIZE + 1);
    cell **board;
    list *moves_list = NULL;
    data new_data;
    int argsCounter;
    char mode = 'I';
    int i;


    printf("Sudoku\n------\n");
    printf("Enter your command:\n");

    while (fgets(command, MAX_CMD_SIZE, stdin) != NULL) { /*not to put max command size but numbers of chars*/
        argsCounter = parseCommand(command, parsedCommand);
        /*put all beneath in function match cmd*/
        if (parsedCommand[0] == NULL)  {
            /*Handles blank line*/
        } else if (strcmp(parsedCommand[0], "solve") == 0) { /*to add freeing moves list and board*/
            board = NULL;
            moves_list = NULL;
            new_data.row_index = 0;
            new_data.col_index =0;
            new_data.col_index = 0;
            mode = 'S';
            board = solveCommand(parsedCommand, mode);
            new_data.board = generateEmptyBoard();
            copyBoard(board, new_data.board);
            moves_list = CreateList(new_data);
            mode = 'S';

        } else if (strcmp(parsedCommand[0], "edit") == 0){ /*to add freeing moves list and board*/
            board = NULL;
            moves_list = NULL;
            new_data.row_index = 0;
            new_data.col_index =0;
            new_data.col_index = 0;
            mode = 'E';
            board = editCommand(parsedCommand, mode);
            new_data.board = generateEmptyBoard();
            copyBoard(board, new_data.board);
            moves_list = CreateList(new_data);

        } else {
            executeCommand(parsedCommand, board, command, argsCounter, mode, moves_list);

        }

        for (i = 0; i < 4; i++){
            parsedCommand[i] = '\0';
        }
        printf("Enter your command:\n");
    }

    return 0;
}