#include "main_aux.h"
#include "parser.h"
#include "game.h"
#include "solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "linked_list.h"


int main() {

    char *parsedCommand[4] = {'\0', '\0', '\0', '\0'};
    char *command = malloc(MAX_CMD_SIZE + 1);
    size_t rows, cols;
    cell **board;
    list *moves_list = NULL;
    data new_data;
    int argsCounter;
    char mode = 'I';
    int i;
    int markErrors = 1;

    printf("Sudoku\n------\n");
    printf("Enter your command:\n");

    while (fgets(command, MAX_CMD_SIZE, stdin) != NULL) { /*not to put max command size but numbers of chars*/
        argsCounter = parseCommand(command, parsedCommand);
        /*put all beneath in function match cmd*/
        if (parsedCommand[0] == NULL)  {
            /*Handles blank line*/
        } else if (strcmp(parsedCommand[0], "solve") == 0) {
            new_data.board = generateEmptyBoard(&rows,&cols);
            new_data.row_index,new_data.col_index,new_data.col_index= 0;
            board = NULL;
            moves_list = NULL;

            board = solveCommand(parsedCommand, &rows, &cols, &markErrors);
            copyBoard(board, new_data.board, rows, cols);
            moves_list = CreateList(new_data);
            mode = 'S';

        } else if (strcmp(parsedCommand[0], "edit") == 0){
            new_data.board = generateEmptyBoard(&rows,&cols);
            new_data.row_index,new_data.col_index,new_data.col_index= 0;
            board = NULL;
            moves_list = NULL;
            DeleteList(moves_list);

            board = editCommand(parsedCommand, &rows, &cols);
            mode = 'E';
        } else {
            executeCommand(parsedCommand, board, command, argsCounter, mode, &rows, &cols, &markErrors, moves_list);

        }
        /*else is redo undo!!!!!!!!!!!! create here linked lst? init in each mode and when del? switching between  modes*/
        for (i = 0; i < 4; i++){
            parsedCommand[i] = '\0';
        }
        printf("Enter your command:\n");
    }

    return 0;
}