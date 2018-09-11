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
    cell **starting_board;
    list *moves_list;

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
            if (mode != 'I'){ /*moving from edit to solve*/
                freeBoard(board);
                freeList(moves_list);
            }
            mode = 'S';
            board = NULL;
            board = solveCommand(parsedCommand, mode);
            starting_board = NULL;
            starting_board = generateEmptyBoard();
            copyBoard(board, starting_board);
            moves_list = NULL;
            moves_list = CreateList(starting_board);



        } else if (strcmp(parsedCommand[0], "edit") == 0){ /*to add freeing moves list and board*/
            if (mode != 'I') { /*moving from solve to edit*/
                freeBoard(board);
                freeList(moves_list);
            }
            mode = 'E';
            board = NULL;
            board = editCommand(parsedCommand, mode);
            starting_board = NULL;
            starting_board = generateEmptyBoard();
            copyBoard(board, starting_board);
            moves_list = NULL;
            moves_list = CreateList(starting_board);
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