#include "main_aux.h"
#include "parser.h"
#include "game.h"
#include "solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "linked_list.h"

int rows = 3;
int cols = 3;
int markErrors = 1;

int main() {

    char *parsedCommand[4] = {'\0', '\0', '\0', '\0'};
    char *command = malloc(MAX_CMD_SIZE + 1);
    cell **board;
    cell **startingBoard;
    list *movesList;
    int argsCounter;
    char mode = 'I';
    int i;


    printf("Sudoku\n------\n");
    printf("Enter your command:\n");

    while (fgets(command, MAX_CMD_SIZE, stdin) != NULL) { /*not to put max command size but numbers of chars*/
        argsCounter = parseCommand(command, parsedCommand);
        /*put all beneath in function match cmd*/
        if (parsedCommand[0] == NULL)  {/*Handles blank line*/
        } else if (strcmp(parsedCommand[0], "solve") == 0) { /*to add freeing moves list and board*/
            if (mode != 'I'){ /*switching mode from edit to solve*/
                free_board(board);
                free_list(movesList);
            }
            mode = 'S';
            board = NULL;
            board = solve_command(parsedCommand, mode);
            startingBoard = NULL;
            startingBoard = generate_empty_board();
            copy_board(board, startingBoard);
            movesList = NULL;
            movesList = create_list(startingBoard);

        } else if (strcmp(parsedCommand[0], "edit") == 0){ /*to add freeing moves list and board*/
            if (mode != 'I') { /*switching mode from solve to edit*/
                free_board(board);
                free_list(movesList);
            }
            mode = 'E';
            board = NULL;
            board = edit_command(parsedCommand, mode);
            startingBoard = NULL;
            startingBoard = generate_empty_board();
            copy_board(board, startingBoard);
            movesList = NULL;
            movesList = create_list(startingBoard);
        } else {
            execute_command(parsedCommand, board, command, argsCounter, mode, movesList);
        }
        for (i = 0; i < 4; i++){
            parsedCommand[i] = '\0';
        }
        printf("Enter your command:\n");
    }

    return 0;
}