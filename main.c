#include "main_aux.h"
#include "parser.h"
#include "game.h"
#include "solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "linked_list.h"

int blockRows;
int blockCols;
int markErrors = 1;
char mode = 'I';

int main() {

    char *parsedCommand[4] = {'\0', '\0', '\0', '\0'};
    char *command = malloc(MAX_CMD_SIZE + 1);
    cell **board;
    cell** tmpBoard;
    cell **startingBoard;
    list *movesList;
    int argsCounter;

    int i;


    printf("Sudoku\n------\n");
    printf("Enter your command:\n");

    while (fgets(command, MAX_CMD_SIZE, stdin) != NULL) { /*not to put max command size but numbers of chars*/
        tmpBoard = NULL;
        argsCounter = parseCommand(command, parsedCommand);
        /*put all beneath in function match cmd*/
        if (parsedCommand[0] == NULL)  {/*Handles blank line*/
        } else if (strcmp(parsedCommand[0], "solve") == 0) {
            tmpBoard = solve_command(parsedCommand, 'S');
            if (tmpBoard != NULL) { /*solve succeeded*/
                if (mode != 'I') { /*switching mode from edit to solve*/
                    free_board(board);
                    free_list(movesList);
                }
                mode = 'S';
                board = NULL;
                board = generate_empty_board();
                copy_board(tmpBoard, board);
                free_board(tmpBoard);
                startingBoard = NULL;
                startingBoard = generate_empty_board();
                copy_board(board, startingBoard);
                movesList = NULL;
                movesList = create_list(startingBoard);
            }
        } else if (strcmp(parsedCommand[0], "edit") == 0){
            tmpBoard = edit_command(parsedCommand, 'E');
            if (tmpBoard != NULL) { /*edit succeeded*/
                if (mode != 'I') { /*switching mode from solve to edit*/
                    free_board(board);
                    free_list(movesList);
                }
                mode = 'E';
                board = NULL;
                board = generate_empty_board();
                copy_board(tmpBoard, board);
                free_board(tmpBoard);
                startingBoard = NULL;
                startingBoard = generate_empty_board();
                copy_board(board, startingBoard);
                movesList = NULL;
                movesList = create_list(startingBoard);
            }
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