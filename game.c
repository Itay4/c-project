
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "main_aux.h"
#include "parser.h"
#include "solver.h"
#include "game.h"
#include "stack.h"
#include "linked_list.h"

#define UNUSED(x) (void)(x) /*REMOVE*/

bool gameOverFlag;

cell **generateEmptyBoard(size_t *rows, size_t *cols){
    size_t i, j, N;
    cell **board = NULL;
    *cols = 3;
    *rows = 3;
    N = 9;
    board = calloc(N, sizeof *board);
    for (i = 0; i < N; i++) {
        board[i] = calloc(N, sizeof **board);
    }
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            board[i][j].number = 0;
        }
    }
    return board;
}

void executeCommand(char *parsedCommand[4], cell **board, char* command, int counter, char mode, size_t *rows, size_t *cols, int *markErrors, list *lst){
    /*
     * Evaluates game command (SET/HINT/VALIDATE/RESTART/EXIT) and calls the relavent function to execute it
     */
    int set_flag = 0;
    data new_data;
    if (strcmp(parsedCommand[0], "set") == 0 && !gameOverFlag && counter == 4 && (mode == 'E' || mode == 'S')) {
        set_flag = set(board, atoi(parsedCommand[1]),atoi(parsedCommand[2]), atoi(parsedCommand[3]), *rows, *cols);
        if (set_flag == 1) {
            new_data.board = generateEmptyBoard(rows, cols);
            copyBoard(board, new_data.board, *rows, *cols);
            new_data.value = atoi(parsedCommand[3]);
            new_data.col_index = atoi(parsedCommand[1]);
            new_data.row_index = atoi(parsedCommand[2]);
            InsertAtTail(new_data, lst);
        }
    } else if (strcmp(parsedCommand[0], "hint") == 0 && !gameOverFlag && counter == 3 && mode == 'S') {
        hint(board, atoi(parsedCommand[1]), atoi(parsedCommand[2]));
    } else if (strcmp(parsedCommand[0], "validate") == 0  && !gameOverFlag && (mode == 'E' || mode == 'S')) {
        validate(board, rows, cols);
    } else if (strcmp(parsedCommand[0], "print_board") == 0 && (mode == 'E' || mode == 'S')) {
        printBoard(board, *rows, *cols, *markErrors);
    } else if (strcmp(parsedCommand[0], "mark_errors") == 0 && mode == 'S') {
        markErrorsCommand(parsedCommand[1], markErrors);
    } else if (strcmp(parsedCommand[0], "autofill") == 0 && mode == 'S') {
        autoFill(board, *rows, *cols, *markErrors);
    } else if (strcmp(parsedCommand[0], "save") == 0 && (mode == 'E' || mode == 'S')) {
        saveCommand(board, rows, cols, parsedCommand[1]);
    } else if (strcmp(parsedCommand[0], "num_solutions") == 0 && (mode == 'E' || mode == 'S')) {
        numSolutions(board, *rows, *cols);
    } else if (strcmp(parsedCommand[0], "undo") == 0) {
        Undo(lst, board, rows, cols, markErrors);

    } else if (strcmp(parsedCommand[0], "redo") == 0) {
        Redo(lst, board, rows, cols, markErrors);
    } else if (strcmp(parsedCommand[0], "exit") == 0) {
        lst = NULL;

        exitGame(command);
    } else { /* Invalid command */
        printf("ERROR: invalid command\n");
    }
}

void initializeBoard(cell board[NUM_OF_ROWS][NUM_OF_COLUMNS]) {
    /*
     * Initializes the sudoku board cells with default values: number - 0, isFixed - false
     */
    int i,j;
    for (i=0; i < 10; i++){
        for (j=0; j < 10; j++){
            board[i][j].isFixed = false;
            board[i][j].number = 0;
        }
    }
}

void printSeperator(int N, int n) {
    /*
     * Helper function to printBoard() which prints block seperators
     */
    char dash = '-';
    int i;
    int count = 4 * N + n + 1;
    for (i = 0; i < count; i++) {
        putchar(dash);
    }
    putchar('\n');
}

void printBoard(cell **board, int rows, int cols, int markErrors) {
    /*
     * Prints the sudoku board according to the format
     */
    int i,j;
    int N = rows * cols;
    for (i = 0; i < N; i++) {
        if (i % rows == 0){
            printSeperator(N, rows);
        }
        for (j=0; j < N; j++) {
            if (j % cols == 0) {
                printf("|");
            }
            printf(" ");
            if (board[i][j].isFixed) {
                printf("%2d", board[i][j].number);
                printf(".");
            }
            else if (board[i][j].asterisk && markErrors) {
                printf("%2d", board[i][j].number);
                printf("*");
            }
            else if (board[i][j].number != UNASSIGNED){
                printf("%2d ", board[i][j].number);
            }
            else{
                printf("   ");
            }
        }
        printf("|\n");
    }
    printSeperator(N, rows);
}

void numSolutions(cell **board, int numOfRows, int numOfCols) {
    int solutionsCounter = countSolutions(board, numOfRows, numOfCols);
    /*int solutionsCounter = countSolutionsRec(board, 0, 0, 0, *rows, *cols);*/
    printf("Number of solutions: %d\n", solutionsCounter);
    if (solutionsCounter == 1) {
        printf("This is a good board!\n");
    } else {
        printf("The puzzle has more than 1 solution, try to edit it further\n");
    }
}

int countSolutionsRec(cell **board, int i, int j, int counter, int numOfRows, int numOfCols) {
    /*
     * Stack implementation
     */
    int N = numOfRows*numOfCols;
    int k;
    if (i == N) {
        i = 0;
        if (++j == N){
            return 1 + counter;
        }
    }
    if (board[i][j].number != 0){
        return countSolutionsRec(board, i+1, j, counter, numOfRows, numOfCols);
    }
    for (k = 1; k <= N; ++k) {
        if (validCheck(board, j, i, k, numOfRows, numOfCols)){
            board[i][j].number = k;
            counter = countSolutionsRec(board, i+1, j, counter, numOfRows, numOfCols);
        }
    }
    board[i][j].number = 0;
    return counter;
}

int countSolutions(cell **board, int numOfRows, int numOfCols) {
    /*
    Stack implementation
    TODO:
        Check if board is erroneous
    */

    int N = numOfRows * numOfCols;

    int returnValue, value;

    SnapShotStruct newSnapshot;

    StackNode* snapshotStack;

    SnapShotStruct currentSnapshot;
    currentSnapshot.i = 0;
    currentSnapshot.j = 0;
    currentSnapshot.counter = 0;
    currentSnapshot.stage = 0;

    push(&snapshotStack, currentSnapshot);

    while (!empty(snapshotStack)) {
        currentSnapshot=top(snapshotStack);
        pop(&snapshotStack);
        if (currentSnapshot.i == N){
            currentSnapshot.i = 0;
            if (++currentSnapshot.j == N) {
                returnValue = currentSnapshot.counter + 1;
                continue;
            }
        }
        if (board[currentSnapshot.i][currentSnapshot.j].number != 0) {
            newSnapshot.i = currentSnapshot.i + 1;
            newSnapshot.j = currentSnapshot.j;
            newSnapshot.counter = currentSnapshot.counter;
            newSnapshot.stage = 0;
            push(&snapshotStack, newSnapshot);
            continue;
        }
        for (value = 1; value <= N; ++value) {
            if (validCheck(board, currentSnapshot.j, currentSnapshot.i, value, numOfRows, numOfCols)){
                board[currentSnapshot.i][currentSnapshot.j].number = value;
                newSnapshot.i = currentSnapshot.i + 1;
                newSnapshot.j = currentSnapshot.j;
                newSnapshot.counter = currentSnapshot.counter;
                newSnapshot.stage = 0;
                push(&snapshotStack, newSnapshot);
                continue;
            }
        }
        board[currentSnapshot.i][currentSnapshot.j].number = 0;
        /*switch (currentSnapshot.stage)
        {
        case 0:
             if (currentSnapshot.i == N){
                 currentSnapshot.i = 0;
                 if (++currentSnapshot.j == N) {
                     returnValue = 1 + currentSnapshot.counter;
                     continue;
                 }
             }
             if (board[currentSnapshot.i][currentSnapshot.j].number != 0) {
                 newSnapshot.i = currentSnapshot.i + 1;
                 newSnapshot.j = currentSnapshot.j;
                 newSnapshot.counter = currentSnapshot.counter;
                 newSnapshot.stage = 0;
                 push(&snapshotStack, newSnapshot);
                 continue;
             }
             for (value = 1; value <= N; ++value) {
                 if (validCheck(board, currentSnapshot.j, currentSnapshot.i, value, numOfRows, numOfCols)){
                     board[currentSnapshot.i][currentSnapshot.j].number = value;
                     newSnapshot.i = currentSnapshot.i + 1;
                     newSnapshot.j = currentSnapshot.j;
                     newSnapshot.counter = currentSnapshot.counter;
                     newSnapshot.stage = 0;
                     push(&snapshotStack, newSnapshot);
                     continue;
                 }
             }
             currentSnapshot.stage = 1;
             push(&snapshotStack, currentSnapshot);

             break;
        case 1:
             newSnapshot.i = currentSnapshot.i + 1;
             newSnapshot.j = currentSnapshot.j;
             newSnapshot.counter = returnValue;
             newSnapshot.stage = 0;
             push(&snapshotStack, newSnapshot);
             continue;
             break;
        }*/
    }
    return returnValue;
}

void saveCommand(cell **board, int rows, int cols, char *filePath) {
    /*
    TODO:
        Check if board is erroneous in Edit mode
        Validate board in Edit mode
        Fix cells in Edit mode
    */
    FILE *fp;
    int i, j, N;
    if ((fp=fopen(filePath, "wb")) == NULL) {
        printf("Error: File cannot be created or modified\n");
    }
    fprintf(fp, "%d %d\n", cols, rows);
    N = rows * cols;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            fprintf(fp, "%d", board[i][j].number);
            if (board[i][j].isFixed) {
                fprintf(fp, ".");
            }
            fprintf(fp, " ");
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    printf("Saved to: %s\n", filePath);
}

void markErrorsCommand(char* value, int *markErrors) {
    if (isInteger(value) && atoi(value) == 0){
        *markErrors = 0;
    } else if (isInteger(value) && atoi(value) == 1){
        *markErrors = 1;
    } else {
        printf("Error: the value should be 0 or 1\n");
    }
}

cell **loadBoard(FILE* fp, size_t *rows, size_t *cols){
    size_t i, j, N;
    cell **board = NULL;
    char *line[256];
    char *token;
    char *delimiter = " \t\r\n";
    if (fgets(line, 256, fp) != NULL){
        token = strtok(line, delimiter);
        *cols = atoi(token);
        token = strtok(NULL, delimiter);
        *rows = atoi(token);
    }
    N = *rows * *cols;
    board = calloc(N, sizeof *board);
    for (i = 0; i < N; i++) {
        board[i] = calloc(N, sizeof **board);
    }
    for (i = 0; i < N; i++) {
        fgets(line, 256, fp);
        token = strtok(line, delimiter);
        for (j = 0; j < N; j++) {
            board[i][j].number = token[0] - '0';
            if (token[1] == '.') {
                board[i][j].isFixed = true;
            }
            token = strtok(NULL, delimiter);
        }
    }
    return board;
}



cell **editCommand(char* parsedCommand[4], size_t *rows, size_t *cols){
    cell **board = NULL;
    if (parsedCommand[1] != '\0'){
        FILE* fp = fopen(parsedCommand[1], "r");
        if (fp) {
            board = loadBoard(fp, rows, cols);
            fclose(fp);
        } else {
            printf("Error: File doesn't exist or cannot be opened\n");
        }
    } else {
        board = generateEmptyBoard(rows, cols);
    }
    printBoard(board, *rows, *cols, 1);
    return board;
}

cell **solveCommand(char* parsedCommand[4], size_t *rows, size_t *cols, int *markErrors){
    cell **board = NULL;
    FILE* fp = fopen(parsedCommand[1], "r");
    if (fp) {
        board = loadBoard(fp, rows, cols);
        printBoard(board, *rows, *cols, *markErrors);
        fclose(fp);
    } else {
        printf("Error: File doesn't exist or cannot be opened\n");
    }
    return board;
}


bool valInBlock(cell **board, int column, int row, int val, int numOfRows, int NumOfCols){
    /*
     * Checks if value exist in the block containing given row and column
     */
    int initial_col, initial_row, col;
    initial_col = column - (column % NumOfCols);
    initial_row = row - (row % numOfRows);
    for (col = initial_col; (col < NumOfCols + initial_col); col++) {
        for (row = initial_row; (row < numOfRows + initial_row); row++) {
            if (board[row][col].number == val) {
                return true;
            }
        }
    }

    return false;
}

bool valInRow(cell **board, int row, int val, int NumOfCols){
    /*
     * Checks if value exist in the given row
     */
    int col;
    for (col = 0; col < NumOfCols; col++) {
        if (board[row][col].number == val) {
            return true;
        }
    }

    return false;
}

bool valInColumn(cell **board, int column, int val, int numOfRows) {
    /*
     * Checks if value exist in the given column
     */
    int row;
    for (row = 0; row < numOfRows; row++) {
        if (board[row][column].number == val) {
            return true;
        }
    }
    return false;
}

bool validCheck(cell **board, int column, int row, int val, int numOfRows, int numOfCols) {
    /*
     * Checks if validation of given value in cell <row,column> according to sudoku rules
     */
    int size = numOfRows * numOfCols;
    return !valInBlock(board, column, row, val, numOfRows, numOfCols) && !valInRow(board, row, val, size) && !valInColumn(board, column, val, size);
}

void gameOver(cell **board){
    /*
     * Checks if board is full
     */
    int col, row;
    bool over = true;
    for (col = 0; col < NUM_OF_COLUMNS; col++) {
        for (row = 0; row < NUM_OF_ROWS; row++) {
            if (board[row][col].number == UNASSIGNED) {
                over = false;
            }
        }
    }
    if (over){
        gameOverFlag = true;
        printf(GAME_OVER);
    }
}
/*
1 0 0 0

0 0 3 0

2 0 0 0

0 0 0 0
*/

void autoFill(cell **board, int rows, int cols, int markErrors)	{
    /*
     * Autofills cells which contain a single legal value
     TODO:
         Check if erroneous
     */
    int i, j, k, candidate;
    int numOfCandidates = 0;
    int maxValue = rows * cols + 1;

    cell **board1;
    int N = rows*cols;
    board1 = calloc(N, sizeof *board1);
    for (i = 0; i < N; i++) {
        board1[i] = calloc(N, sizeof **board1);
    }
    for (i = 0; i < N; i++){
        for (j = 0; j < N; j++){
            board1[i][j].number = board[i][j].number;
        }
    }
    for (i = 0; i < N; i++){
        for (j = 0; j < N; j++){
            if (board1[i][j].number == UNASSIGNED){
                for (k = 1; k < maxValue; k++){

                    if (validCheck(board1, j, i, k, rows, cols)){
                        candidate = k;
                        numOfCandidates++;
                    }
                }
                if (numOfCandidates == 1){
                    board[i][j].number = candidate;
                    printf("Cell <%d,%d> set to %d\n", i, j, candidate);
                }
                numOfCandidates = 0;
            }
        }
    }
    printBoard(board, rows, cols, markErrors);
}

int set(cell **board, int column, int row, int val, int numOfRows, int numOfCols)	{
    /*
     * Sets the value val to cell <row,column> on the sudoku board if its following the game rules
     * return 1 if successful, else 0
     */
    if (val == 0){
        board[row - 1][column - 1].number = UNASSIGNED;
        /*printBoard(board);*/
    } else if (board[row - 1][column - 1].isFixed) {
        printf(FIXED_ERROR);
        return 0;
    } else if (board[row - 1][column - 1].number == val) {

        /*printBoard(board);*/
    } else if (validCheck(board, column - 1, row - 1, val, numOfRows, numOfCols)){
        board[row - 1][column - 1].number = val;
        /*printBoard(board);*/
    } else{
        printf(INVALID_ERROR);
        return 0;
    }
    gameOver(board);
    return 1;
}

void validate(cell **board, size_t *rows, size_t *cols){
    /*
     * returns true if current board is sSolvable according to the deterministic backtracking and updates stored solution

    cell copy_of_user_board[NUM_OF_ROWS][NUM_OF_COLUMNS];
    bool solvable;
    copyBoard(user_board, copy_of_user_board);
    solvable = deterministicBacktrack(copy_of_user_board);
    if (solvable){
        printf("Validation passed: board is solvable\n");
        copyBoard(copy_of_user_board, solved_board);
    } else{
        printf("Validation failed: board is unsolvable\n");
    }
    printf("here%d", board[*rows][*cols].number);
    ILP(board, *rows, *cols);
    */





}

void hint(cell **board, int column, int row){
    /*
     * Prints the value of the cell <row,column> on the last solved sudoku board
     */
    int hint;
    hint = board[row-1][column-1].number;
    printf("Hint: set cell to %d\n", hint);
}