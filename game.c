
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


#define UNUSED(x) (void)(x) /*REMOVE*/

bool gameOverFlag;

extern int rows;
extern int cols;
extern int mark_errors;


cell **generateEmptyBoard(){
    int i, j, N;
    cell **board = NULL;
    N = rows * cols;
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

void executeCommand(char *parsedCommand[4], cell **board, char* command, int counter, char mode, list *lst){
    /*
     * Evaluates game command (SET/HINT/VALIDATE/RESTART/EXIT) and calls the relavent function to execute it
     */
    
    int set_flag = 0;
    int fill_flag = 0;
    cell ** board_after;

    if (strcmp(parsedCommand[0], "set") == 0 && !gameOverFlag && counter == 4 && (mode == 'E' || mode == 'S')) {
        set_flag = set(board, atoi(parsedCommand[1]),atoi(parsedCommand[2]), atoi(parsedCommand[3]), mode);
        if (set_flag == 1) {
            board_after = generateEmptyBoard(rows, cols);
            copyBoard(board, board_after);
            InsertAtTail(board_after, lst);
        }
    } else if (strcmp(parsedCommand[0], "hint") == 0 && !gameOverFlag && counter == 3 && mode == 'S') {
        hint(board, atoi(parsedCommand[1]), atoi(parsedCommand[2]));
    } else if (strcmp(parsedCommand[0], "validate") == 0  && !gameOverFlag && (mode == 'E' || mode == 'S')) {
        validate(board);
    } else if (strcmp(parsedCommand[0], "print_board") == 0 && (mode == 'E' || mode == 'S')) {
        printBoard(board);
    } else if (strcmp(parsedCommand[0], "mark_errors") == 0 && mode == 'S') {
        markErrorsCommand(parsedCommand[1]);
    } else if (strcmp(parsedCommand[0], "autofill") == 0 && mode == 'S') {
        fill_flag = autoFill(board);
        if (fill_flag == 1) {
            board_after = generateEmptyBoard(rows, cols);
            copyBoard(board, board_after);
            InsertAtTail(board_after, lst);
        }
    } else if (strcmp(parsedCommand[0], "save") == 0 && (mode == 'E' || mode == 'S')) {
        saveCommand(board, parsedCommand[1], mode);
    } else if (strcmp(parsedCommand[0], "num_solutions") == 0 && (mode == 'E' || mode == 'S')) {
        numSolutions(board);
    } else if (strcmp(parsedCommand[0], "undo") == 0) {
        Undo(lst, board);

    } else if (strcmp(parsedCommand[0], "redo") == 0) {
        Redo(lst, board);
    } else if (strcmp(parsedCommand[0], "reset") == 0) {
        reset(lst, board);
    } else if (strcmp(parsedCommand[0], "exit") == 0) {
        freeBoard(board);
        freeList(lst);
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

void printBoard(cell **board) { /*update with mode- print always atrisk in edit*/
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
            else if (board[i][j].asterisk && mark_errors /*|| edit*/) {
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

void validate(cell **board) {
    /*
     * Validates the sudoku board is solvable
     */
    cell **copyBoard;
    int solvable;
    /* if board contains erroneous values {
        printf("Error: board contains erroneous values\n");
    }*/
    copyBoard = duplicate_board(board);
    solvable = 1;
    solvable = ILP(board, copyBoard);

    if (solvable == 1) {
        printf("Validation passed: board is solvable\n");
    } else {
        printf("Validation failed: board is unsolvable\n");
    }
    freeBoard(copyBoard);
}

void numSolutions(cell **board) {
    /* Counts the number of solutions of sudoku board */


    int solutionsCounter = countSolutions(board);
    if (check_board_erroneous(board)){
        printf(ERRONEOUS_ERROR);
    }
    printf("Number of solutions: %d\n", solutionsCounter);
    if (solutionsCounter == 1) {
        printf("This is a good board!\n");
    } else {
        printf("The puzzle has more than 1 solution, try to edit it further\n");
    }
}

int countSolutions(cell** board) {
    /* Helper function to count solutions of sudoku board */
    int numOfSolutions;
    int* unassignedsArray;
    cell** boardCopy = generateEmptyBoard();
    copyBoard(board, boardCopy);

    unassignedsArray = get_next_play(boardCopy);
    if (unassignedsArray[0] == -1) {
        free(unassignedsArray);
        return 0;
    }
    numOfSolutions = deterministic_backtrack(boardCopy, unassignedsArray[0], unassignedsArray[1]);
    freeBoard(boardCopy);
    free(unassignedsArray);
    return numOfSolutions;
}

void saveCommand(cell **board, char *filePath,char mode) {
    /*
    TODO:
        remove remarks... waiting for validate
    */

    FILE *fp;
    int i, j, N;

    if (mode == 'E') {
        if (checkBoardErroneous(board)) {
            printf("Error: board contains erroneous values\n");
            return;
        }
      /*
        else if (validate(board) == false) {
            printf("Error: board validation failed\n");
            return;
      }
        */
    }

    if ((fp = fopen(filePath, "wb")) == NULL) {
        printf("Error: File cannot be created or modified\n");
    }

    fprintf(fp, "%d %d\n", cols, rows);
    N = rows * cols;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            fprintf(fp, "%d", board[i][j].number);
            if (board[i][j].isFixed || mode == 'E') {
                fprintf(fp, ".");
            }
            else if (board[i][j].asterisk) {
                fprintf(fp, "*");
            }
            fprintf(fp, " ");
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    printf("Saved to: %s\n", filePath);
}

void markErrorsCommand(char* value) {
    if (isInteger(value) && atoi(value) == 0){
        mark_errors = 0;
    } else if (isInteger(value) && atoi(value) == 1){
        mark_errors = 1;
    } else {
        printf("Error: the value should be 0 or 1\n");
    }
}

cell **loadBoard(FILE* fp, char mode){/*add char mode- in edit need to clear all fixed*/
    size_t i, j, N;
    cell **board = NULL;
    char line[257];
    char *token;
    char *delimiter = " \t\r\n";
    if (fgets(line, 256+1, fp) != NULL){
        token = strtok(line, delimiter);
        cols = atoi(token);
        token = strtok(NULL, delimiter);
        rows = atoi(token);
    }
    N = rows * cols;
    board = calloc(N, sizeof *board);
    for (i = 0; i < N; i++) {
        board[i] = calloc(N, sizeof **board);
    }
    for (i = 0; i < N; i++) {
        fgets(line, 256, fp);
        token = strtok(line, delimiter);
        for (j = 0; j < N; j++) {
            board[i][j].number = token[0] - '0';
            if ((token[1] == '.') && (mode == 'S')) {
                    board[i][j].isFixed = true;
            }
            else if (token[1] == '*') {
                board[i][j].asterisk = true;
                }
            token = strtok(NULL, delimiter);
        }
    }
    return board;
}



cell **editCommand(char* parsedCommand[4], char mode){
    cell **board = NULL;
    mark_errors = 1;
    if (parsedCommand[1] != '\0'){
        FILE* fp = fopen(parsedCommand[1], "r");
        if (fp) {
            board = loadBoard(fp, mode);
            fclose(fp);
        } else {
            printf("Error: File cannot be opened\n");
        }
    } else {
        board = generateEmptyBoard(rows, cols);
    }
    printBoard(board);
    return board;
}

cell **solveCommand(char* parsedCommand[4],char mode){
    cell **board = NULL;
    FILE* fp = fopen(parsedCommand[1], "r");
    if (fp) {
        board = loadBoard(fp,mode);
        printBoard(board);
        fclose(fp);
    } else {
        printf("Error: File doesn't exist or cannot be opened\n");
    }
    return board;
}


bool valInBlock(cell **board, int column, int row, int val){
    /*
     * Checks if value exist in the block containing given row and column
     */

    bool val_exist = false;
    int initial_col, initial_row, col_index, row_index;
    /*consider to put in get index of cols and rows initial block*/
    int block_number_cols = 1 + ((column - 1) / cols);
    int block_number_rows = 1 + ((row - 1) / rows);

    initial_col = cols * (block_number_cols - 1);
    initial_row = rows * (block_number_rows - 1);


    for (col_index = initial_col; (col_index < cols + initial_col); col_index++) {
        for (row_index = initial_row; (row_index < rows + initial_row); row_index++) {
            if((col_index == column - 1) && (row_index == row - 1)) { /*not checking cell to be changed*/}
            else if (board[row_index][col_index].number == val) {
                board[row_index][col_index].asterisk= true;
                val_exist = true;
            }
        }
    }
    return val_exist;
}

bool valInRow(cell **board, int column, int row, int val){
    /*
     * Checks if value exist in the given row
     */

    int N = rows * cols;
    bool val_exist = false;
    int col_index;
    for (col_index = 0; col_index < N; col_index++) {
        if(col_index == column - 1) {/* not checking cell to be changed*/}
        else if (board[row - 1][col_index].number == val) {
            val_exist = true;
            if (board[row - 1][col_index].isFixed == false) {
                board[row - 1][col_index].asterisk = true;
            }
        }
    }
    return val_exist;
}

bool valInColumn(cell **board, int column, int row, int val) {
    /*
     * Checks if value exist in the given column
     */
    int N = rows * cols;
    bool val_exist = false;
    int row_index;
    for (row_index = 0; row_index < N; row_index++) {
        if(row_index == row - 1){/*not checking cell to be changed*/}
        else if (board[row_index][column - 1].number == val) {
            val_exist = true;
            if (board[row_index][column - 1].isFixed == false) {
                board[row_index][column - 1].asterisk = true;
            }
        }
    }
    return val_exist;
}

bool validCheck(cell **board, int column, int row, int val) {
    /*
     * Checks if validation of given value in cell <row,column> according to sudoku rules
     * marks aserisks
     */

    if(valInBlock(board, column, row, val) | valInRow(board, column, row, val) | valInColumn(board, column, row, val)){
        board[row - 1][column - 1].asterisk = true;
        return false;
    }
    else{
        board[row - 1][column - 1].asterisk = false;
        return true;
    }

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

int autoFill(cell **board)  {
    /*
     * Autofills cells which contain a single legal value

     */
    int i, j, k, candidate;
    int fill_flag = 0;
    int numOfCandidates = 0;
    int maxValue = rows * cols + 1;
    cell **copy_of_board;
    int N = rows * cols;
    if (checkBoardErroneous(board)){
        printf("Error: board contains erroneous values\n");
        return 0;
    }
    copy_of_board = generateEmptyBoard();
    copyBoard(board, copy_of_board);
    for (i = 0; i < N; i++){
        for (j = 0; j < N; j++){
            if (copy_of_board[i][j].number == UNASSIGNED){
                for (k = 1; k < maxValue; k++){
                    if (validCheck(copy_of_board, j + 1, i + 1, k)){
                        candidate = k;
                        numOfCandidates++;
                    }
                }
                if (numOfCandidates == 1){
                    board[i][j].number = candidate;
                    fill_flag = 1;
                    printf("Cell <%d,%d> set to %d\n", (j+1), (i+1), candidate);
                }
                numOfCandidates = 0;
            }

        }
    }
    printBoard(board);
    return fill_flag;
}

void validateRisks(cell **board, int column, int row) {
    int initial_col, initial_row, col_index, row_index;
    int N = rows*cols;

    /*maybe to put in get index of cols and rows inita block*/
    int block_number_cols = 1 + ((column - 1) / cols);
    int block_number_rows = 1 + ((row - 1) / rows);
    /*to check if always work*/
    initial_col = cols * (block_number_cols - 1);
    initial_row = rows * (block_number_rows - 1);
    for (col_index = initial_col; (col_index < cols + initial_col); col_index++) { /*block*/
        for (row_index = initial_row; (row_index < rows + initial_row); row_index++) {
            if (col_index == (column-1) && row_index == (row-1)) {
                /*not checking cell to be changed*/
            }
            else if (board[row_index][col_index].asterisk){
                if(validCheck(board, col_index + 1, row_index + 1, board[row_index][col_index].number)){
                    board[row_index][col_index].asterisk = false;
                }
            }
        }
    }
    for (col_index = 0; (col_index < N); col_index++) { /*row*/
        if (col_index == column - 1) {
        }
        else if (board[row - 1][col_index].asterisk){
            if (validCheck(board, col_index + 1, row, board[row - 1][col_index].number)){
                board[row - 1][col_index].asterisk = false;
            }
        }

    }

    for (row_index = 0; (row_index < N); row_index++) {  /*col*/
        if (row_index == row - 1){
        }
        else if (board[row_index][column - 1].asterisk){
            if( validCheck(board, column , row_index + 1, board[row_index][column - 1].number)){
                board[row_index][column - 1].asterisk = false;
            }
        }
    }
}

int setInEditMode(cell **board, int column, int row, int val) {
   /* edit inputvalid
    * if (inputValid == 0){
        printf(INVALID_ERROR);
        return 0;
    }*/
    if (board[row - 1][column - 1].number == val) {
        printBoard(board);
        return 0;
    }
    else if (val == 0) {
        board[row - 1][column - 1].number = UNASSIGNED;
        board[row - 1][column - 1].asterisk = false;
        validateRisks(board, column, row);
        printBoard(board);
    }
    else{
        validCheck(board, column, row, val); /*valid check to updated astrisk*/
        board[row - 1][column - 1].number = val;
        validateRisks(board, column, row);
        printBoard(board);
    }
    return 1;
}

int setInSolveMode(cell **board, int column, int row, int val) {
    /* edit inputvalid
     * if (inputValid == 0){
         printf(INVALID_ERROR);
         return 0;
     }*/
     if (board[row - 1][column - 1].isFixed) {
        printf(FIXED_ERROR);
        return 0;
     } else if (board[row - 1][column - 1].number == val) {
         printBoard(board);
         return  0;
    } else if (val == 0) {
        board[row - 1][column - 1].number = UNASSIGNED;
        board[row - 1][column - 1].asterisk = false;
        validateRisks(board, column,row);
        printBoard(board);
    } else {
        validCheck(board, column, row, val); /*make valid check to updated astrisk*/
        board[row - 1][column - 1].number = val;
        validateRisks(board, column,row);
        printBoard(board);
    }
    gameOver(board);
    return 1;
}

int set(cell **board, int column, int row, int val, char mode) {
    /*
     * Sets the value val to cell <row,column> on the sudoku board if its following the game rules
     * return 1 if successful, else 0
     */
    int set_outcome;
    if (mode == 'E') {
        set_outcome = setInEditMode(board, column, row, val);
    } else if (mode == 'S') {
        set_outcome = setInSolveMode(board, column, row, val);
    }
    return set_outcome;
}






bool checkBoardErroneous(cell **board){
    int col_index, row_index = 0;
    int N = rows * cols;
    for (row_index = 0; row_index < N; row_index++) {
        for (col_index = 0; col_index < N; col_index++) {
            if (board[row_index][col_index].asterisk) {
                return true;
            }
        }
    }
    return false;
}




/*void validate(cell **board){

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


}*/

void hint(cell **board, int column, int row) {
    /*
     * Prints the value of the cell <row,column> on the last solved sudoku board
     */
    /*TODO: after ILP solver working unmark code-*/
    /* int hint;
    cell **solved_board = NULL; */
    if (checkBoardErroneous(board)) {
        printf("Error: board contains erroneous values\n");
        return;
    }
    if (board[row - 1][column - 1].isFixed) {
        printf("Error: cell is fixed\n");
        return;;
    }
    if (board[row - 1][column - 1].number == 0) {
        printf("Error: cell already contains a value\n");
        return;
    }
    /*solved_board = validate(board)
     * if (solved_board == NULL){
     * printf("Error: board is unsolvable\n");
     * return;
     * }
    else{
         hint = solved_board_[row-1][column-1].number;
        printf("Hint: set cell to %d\n", hint);
        }*/
}
