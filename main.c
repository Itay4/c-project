/** @file main.c
 *  @brief main source file.
 *
 *  main source file which is used to run the game, get user input and follow accordingly.
 *
 *  @author Itay Keren (itaykeren)
 *  @author Rotem Bar (rotembar)
 *
 */


/* -- Includes -- */
#include "main_aux.h"
#include "parser.h"
#include "game.h"
#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


/* -- Global Variables -- */
int blockRows;
int blockCols;
int markErrors = 1;
char mode = 'I';
bool gameOver = false;

int main() {
    char* parsedCommand[4] = {'\0', '\0', '\0', '\0'};
    char* command = malloc(MAX_CMD_SIZE + 2);
    cell** board;
    cell** tmpBoard;
    cell** startingBoard;
    list* movesList;
    int argsCounter, i, tempRows, tempCols, switchRows, switchCols;
    bool invalidCmd = false;
    if (command == NULL) {
        memory_error("Memory allocation failed\n");
        exit(0);
    }
    srand(time(NULL)); /* setting random seed */
    printf("Sudoku\n------\n");
    printf("Enter your command:\n");
    while (fgets(command, MAX_CMD_SIZE + 2, stdin) != NULL) {
        if (!strchr(command, '\n')) { /* command contains more than 256 chars */
            while (!strchr(command, '\n')  && fgets(command, MAX_CMD_SIZE + 2, stdin)); /* cleaning buffer */
            invalidCmd = true;
            printf(INVALID_ERROR);

        }
        argsCounter = parseCommand(command, parsedCommand);
        tmpBoard = NULL;
        if (parsedCommand[0] == NULL) {/* Handles blank line */
        } else if (strcmp(parsedCommand[0], "solve") == 0 && !invalidCmd) {
            tempRows = blockRows;
            tempCols = blockCols;
            tmpBoard = solve_command(parsedCommand, 'S');
            if (tmpBoard != NULL) { /* solve succeeded */
                if (mode != 'I' || gameOver) { /* switching mode from edit or old solve to new solve */
                    switchRows = blockRows;
                    switchCols = blockCols;
                    blockRows = tempRows;
                    blockCols = tempCols;
                    free_board(board);
                    free_list(movesList);
                    blockRows = switchRows;
                    blockCols = switchCols;
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
            tempRows = blockRows;
            tempCols = blockCols;
            tmpBoard = edit_command(parsedCommand, 'E');
            if (tmpBoard != NULL) { /* edit succeeded */
                if (mode != 'I' || gameOver) { /* switching mode from solve or old edit to new edit */
                    switchRows = blockRows;
                    switchCols = blockCols;
                    blockRows = tempRows;
                    blockCols = tempCols;
                    free_board(board);
                    free_list(movesList);
                    blockRows = switchRows;
                    blockCols = switchCols;
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
        if (mode != 'I') { /* freeing memory if not in 'I' mode */
            free_board(board);
            free_list(movesList);
        }
        exit_game(command);
    }
    else if (ferror(stdin)) { /* Error */
        if (mode != 'I') { /* freeing memory if not in 'I' mode */
            free_board(board);
            free_list(movesList);
        }
        memory_error("main");
        exit_game(command);
    }
    return 0;
}