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
bool gameOver = false;

int main() {
    char *parsedCommand[4] = {'\0', '\0', '\0', '\0'};
    char *command = malloc(MAX_CMD_SIZE + 2);
    cell **board;
    cell** tmpBoard;
    cell **startingBoard;
    list *movesList;
    int argsCounter, i;
    bool invalidCmd = false;

    srand(time(NULL));
    printf("Sudoku\n------\n");
    printf("Enter your command:\n");

    while (fgets(command, MAX_CMD_SIZE + 2, stdin) != NULL) {
        if (!strchr(command, '\n')) {
            while (!strchr(command, '\n')  && fgets(command, MAX_CMD_SIZE + 2, stdin));
            invalidCmd = true;
            printf(INVALID_ERROR);

        }
        argsCounter = parseCommand(command, parsedCommand);
        tmpBoard = NULL;

        /*put all beneath in function match cmd*/
        if (parsedCommand[0] == NULL) {/*Handles blank line*/
        } else if (strcmp(parsedCommand[0], "solve") == 0 && !invalidCmd) {
            tmpBoard = solve_command(parsedCommand, 'S');
            if (tmpBoard != NULL) { /*solve succeeded*/
                if (mode != 'I' || gameOver) { /*switching mode from edit to solve*/
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
        } else if (strcmp(parsedCommand[0], "edit") == 0 && !invalidCmd) {
            tmpBoard = edit_command(parsedCommand, 'E');
            if (tmpBoard != NULL) { /*edit succeeded*/
                if (mode != 'I' || gameOver) { /*switching mode from solve to edit*/
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
        }
        else if (!invalidCmd){
            execute_command(parsedCommand, board, command, argsCounter, mode, movesList);
        }
        for (i = 0; i < 4; i++){
            parsedCommand[i] = '\0';
        }
        invalidCmd = false;
        printf("Enter your command:\n");
    }
    if (feof(stdin)) { /* EOF */
        if (mode != 'I') { /*switching mode from solve to edit*/
            free_board(board);
            free_list(movesList);
        }
        exit_game(command);
    }
    else if (ferror(stdin)) { /* Error */
            if (mode != 'I') { /*switching mode from solve to edit*/
                free_board(board);
                free_list(movesList);
            }
        memory_error("main");
        exit_game(command);
    }


    return 0;
}
