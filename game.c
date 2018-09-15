#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "main_aux.h"
#include "parser.h"
#include "solver.h"
#include "game.h"
#include "stack.h"
#include "linked_list.h"
#include <unistd.h>

extern int blockRows;
extern int blockCols;
extern int markErrors;
extern char mode;
extern bool gameOver;

int count_empty_cells(cell ** board) {
    int i, j;
    int counter = 0;
    int N = blockRows * blockCols;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (board[i][j].number != UNASSIGNED) {
                counter++;
            }
        }
    }
    return counter;
}



void empty_board(cell ** board) {
    /* empty current board by setting all values to unassigned*/
    int i, j;
    int N = blockRows * blockCols;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            board[i][j].number = UNASSIGNED;
            board[i][j].asterisk = false;
            board[i][j].isFixed = false;
        }
    }
}

bool board_is_empty(cell ** board) {
    /*returns true if board is filled only with unassigned values*/

    int N = blockRows * blockCols;
    if (count_empty_cells(board) != N){
        return false;
    }
    return true;
}

cell** generate_empty_board(){
    /*allocates memmory for a board and reset it with unassigned values;*/
    int i, N;
    cell **board = NULL;
    N =  blockRows * blockCols;
    board = calloc(N, sizeof (*board));
    for (i = 0; i < N; i++) {
        board[i] = calloc(N, sizeof (**board));
    }
    empty_board(board);
    return board;
}

void copy_board(cell **source_board, cell **new_board){
    /*
     * Copying source sudoku board to new sudoku board.
     */
    int i,j;
    int N = blockRows * blockCols;


    for (i=0; i < N; i++) {
        for (j=0; j < N; j++){
            /*printf("I: %d, J: %d\n", i, j);*/

            new_board[i][j].number = source_board[i][j].number;
            new_board[i][j].asterisk = source_board[i][j].asterisk;
            new_board[i][j].isFixed = source_board[i][j].isFixed;

        }
    }
}

void mark_asterisks(cell **board) {
    int i, j;
    int N = blockCols * blockRows;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (board[i][j].number != 0) {
                valid_check(board, j + 1, i + 1, board[i][j].number);
            }
        }
    }
}

void update_moves_list(cell** board, list *lst) {
    cell **boardAfter;
    boardAfter = generate_empty_board(blockRows, blockCols);
    copy_board(board, boardAfter);
    insert_at_tail(boardAfter, lst);
    return;
}


void execute_command(char *parsedCommand[4], cell **board, char* command, int counter, char mode, list *lst){
    /*
     * Evaluates game command (SET/HINT/VALIDATE/RESTART/EXIT) and calls the relavent function to execute it
     */
    bool cmdExecuted = false;
    if (strcmp(parsedCommand[0], "set") == 0 && counter == 4 && (mode == 'E' || mode == 'S')) {
        if ((!is_integer(parsedCommand[1])) || (!is_integer(parsedCommand[2])) || (!is_integer(parsedCommand[3]))){
            printf(VALUE_RANGE_ERROR, blockCols * blockRows);
            return;
        }
        cmdExecuted = set(board, atoi(parsedCommand[1]),atoi(parsedCommand[2]), atoi(parsedCommand[3]), mode);
        if (cmdExecuted) {
            update_moves_list(board, lst);
        }
    } else if (strcmp(parsedCommand[0], "hint") == 0 &&  counter == 3 && mode == 'S') {
        if ((!is_integer(parsedCommand[1])) || (!is_integer(parsedCommand[2]))){
            printf(VALUE_RANGE_ERROR, blockCols*blockRows);
            return;
        }
        hint(board, atoi(parsedCommand[1]), atoi(parsedCommand[2]));
    } else if (strcmp(parsedCommand[0], "validate") == 0  && (mode == 'E' || mode == 'S')) {
        validate(board);
    } else if ((strcmp(parsedCommand[0], "print_board") == 0) && (mode == 'E' || mode == 'S')) {
        print_board(board, mode);
    } else if (strcmp(parsedCommand[0], "mark_errors") == 0 && counter==2 && (mode == 'S')) {
        if (!is_integer(parsedCommand[1])) {
            printf(MARK_ERROR_ERROR);
            return;
        }
        mark_errors_command(atoi(parsedCommand[1]));
    } else if (strcmp(parsedCommand[0], "autofill") == 0 && mode == 'S') {
        cmdExecuted = auto_fill(board);
        if (cmdExecuted) {
            update_moves_list(board, lst);
        }
    } else if (strcmp(parsedCommand[0], "save") == 0 && (mode == 'E' || mode == 'S')) {
        save_command(board, parsedCommand[1], mode);
    } else if (strcmp(parsedCommand[0], "num_solutions") == 0 && (mode == 'E' || mode == 'S')) {
        num_solutions(board);
    } else if (strcmp(parsedCommand[0], "generate") == 0 && counter == 3 && mode == 'E' ) {
        if ((!is_integer(parsedCommand[1])) || (!is_integer(parsedCommand[2]))){
            printf(VALUE_RANGE_ERROR, blockCols * blockRows);
            return;
        }
        cmdExecuted = generate_randomized_solved_board(board, atoi(parsedCommand[1]), atoi(parsedCommand[2]));
        if (cmdExecuted) {
            update_moves_list(board, lst);
        }
    } else if (((strcmp(parsedCommand[0], "undo") == 0)) && (mode == 'E' || mode == 'S')) {
        undo(lst, board, mode);
    } else if (((strcmp(parsedCommand[0], "redo") == 0)) && (mode == 'E' || mode == 'S')) {
        redo(lst, board, mode);
    } else if (((strcmp(parsedCommand[0], "reset") == 0)) && (mode == 'E' || mode == 'S')) {
        reset(lst, board, mode);
    } else if (strcmp(parsedCommand[0], "exit") == 0) {
        if (mode != 'I' || gameOver) {
            free_board(board);
            free_list(lst);
        }
        exit_game(command);
    } else {
        printf(INVALID_ERROR);
    }
}

void print_separator(int N, int m) {
    /*
     * Helper function to printBoard() which prints block separators
     */
    char dash = '-';
    int i;
    int count = 4 * N + m + 1;
    for (i = 0; i < count; i++) {
        putchar(dash);
    }
    putchar('\n');
}

void print_board(cell **board, char mode) {
    /*
     * Prints the sudoku board according to the format
     */
    int i,j;
    int N = blockRows * blockCols;
    for (i = 0; i < N; i++) {
        if (i % blockRows == 0){
            print_separator(N, blockRows);
        }
        for (j=0; j < N; j++) {
            if (j % blockCols == 0) {
                printf("|");
            }
            printf(" ");
            if (board[i][j].isFixed) {
                printf("%2d", board[i][j].number);
                printf(".");
            }
            else if ((board[i][j].asterisk) && (markErrors || mode == 'E')) {
                printf("%2d", board[i][j].number);
                printf("*");
            }
            else if (board[i][j].number != UNASSIGNED){
                printf("%2d ", board[i][j].number);
            }
            else{ /*printing blank spaces for UNASSIGNED*/
                printf("   ");
            }
        }
        printf("|\n");
    }
    print_separator(N, blockRows);
}

void validate(cell **board) {
    /*
     * Validates the sudoku board is solvable
     */
    cell **copyBoard;
    int solvable;
    if (check_board_erroneous(board)) {
        printf(ERRONEOUS_ERROR );
        return;
    }
    copyBoard = duplicate_board(board);
    solvable = ILP(board, copyBoard);

    if (solvable == true) {
        printf("Validation passed: board is solvable\n");
    } else {
        printf("Validation failed: board is unsolvable\n");
    }
    free_board(copyBoard);
}

void num_solutions(cell **board) {
    /* Counts the number of solutions of sudoku board */
    int solutionsCounter = count_solutions(board);
    if (check_board_erroneous(board)) {
        printf(ERRONEOUS_ERROR);
    }
    printf("Number of solutions: %d\n", solutionsCounter);
    if (solutionsCounter == 1) {
        printf("This is a good board!\n");
    } else if (solutionsCounter > 1) {
        printf("The puzzle has more than 1 solution, try to edit it further\n");
    }
}

int count_solutions(cell** board) {
    /* Helper function to count solutions of sudoku board */
    int numOfSolutions;
    int* unassignedsArray;
    cell** boardCopy = generate_empty_board();
    copy_board(board, boardCopy);
    unassignedsArray = get_next_play(boardCopy);
    if (unassignedsArray[0] == -1) {
        free(unassignedsArray);
        return 0;
    }
    numOfSolutions = deterministic_backtrack(boardCopy, unassignedsArray[0], unassignedsArray[1]);
    free_board(boardCopy);
    free(unassignedsArray);
    return numOfSolutions;
}

void save_command(cell **board, char *filePath,char mode) {
    FILE *fp;
    int i, j, N;
    cell ** copyOfBoard;
    bool solvable;

    if (mode == 'E') {
        if (check_board_erroneous(board)) {
            printf(ERRONEOUS_ERROR);
            return;
        }
        copyOfBoard = generate_empty_board();
        solvable = ILP(board,copyOfBoard);
        free_board(copyOfBoard);
        if (solvable == false) {
              printf("Error: board validation failed\n");
              return;
        }

    }

    if ((fp = fopen(filePath, "wb")) == NULL) {
        printf("Error: File cannot be created or modified\n");
    }

    fprintf(fp, "%d %d\n", blockRows, blockCols);
    N = blockRows * blockCols;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            fprintf(fp, "%d", board[i][j].number);
            if ((board[i][j].isFixed || mode == 'E') && (board[i][j].number != UNASSIGNED)) {
                fprintf(fp, ".");
            }
            fprintf(fp, " ");
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    printf("Saved to: %s\n", filePath);
}

void mark_errors_command(int value) {
    if (value == 0) {
        markErrors = 0;
    } else if (value == 1) {
        markErrors = 1;

    }
}

cell **load_board(FILE* fp, char mode){/*add char mode- in edit need to clear all fixed*/
    int i, j, N;
    cell **board = NULL;
    char line[257];
    char *token;
    char *delimiter = " \t\r\n";
    if (fgets(line, 257, fp) != NULL){

        token = strtok(line, delimiter);
        blockRows = atoi(token);
        token = strtok(NULL, delimiter);
        blockCols = atoi(token);
    }
    N = blockRows * blockCols;
    board = calloc(N, sizeof (*board));
    for (i = 0; i < N; i++) {
        board[i] = calloc(N, (sizeof **board));
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
    mark_asterisks(board);
    return board;
}

cell ** edit_command(char* parsedCommand[4], char mode){
    cell **board = NULL;
    FILE* fp = NULL;
    if (parsedCommand[1] != '\0'){
        fp = fopen(parsedCommand[1], "r");
        if (fp != NULL) {
            board = load_board(fp, mode);
            fclose(fp);
        }
        else {
            printf("Error: File cannot be opened\n");
            return board;
        }
    } else {
        blockRows = 3;
        blockCols = 3;
        board = generate_empty_board();
    }
    markErrors = 1;
    print_board(board, mode);
    return board;
}

cell **solve_command(char* parsedCommand[4],char mode){
    cell **board = NULL;
    FILE* fp = NULL;
    if (parsedCommand[1] == '\0'){
        printf(INVALID_ERROR);
        return board;
    }
    fp = fopen(parsedCommand[1], "r");
    if (fp != NULL) {
        board = load_board(fp, mode);
        fclose(fp);
        print_board(board, mode);
    } else {
        printf("Error: File doesn't exist or cannot be opened\n");
    }
    return board;
}

bool val_in_block(cell **board, int column, int row, int val){
    /*
     * Checks if value exist in the block containing given row and column
     */
    bool valExist = false;
    int initialCol, initialRow, colIndex, rowIndex;
    initialCol = get_block_col_index(column);
    initialRow = get_block_row_index(row);
    for (colIndex = initialCol; (colIndex < blockCols + initialCol); colIndex++) {
        for (rowIndex = initialRow; (rowIndex < blockRows + initialRow); rowIndex++) {
            if((colIndex == column - 1) && (rowIndex == row - 1)) { /*not checking cell to be changed*/}
            else if (board[rowIndex][colIndex].number == val) {
                board[rowIndex][colIndex].asterisk= true;
                valExist = true;
            }
        }
    }
    return valExist;
}

bool val_in_row(cell **board, int column, int row, int val){
    /*
     * Checks if value exist in the given row
     */
    int N = blockRows * blockCols;
    bool valExist = false;
    int colIndex;
    for (colIndex = 0; colIndex < N; colIndex++) {
        if(colIndex == column - 1) {/* not checking cell to be changed*/}
        else if (board[row - 1][colIndex].number == val) {
            valExist = true;
            if (board[row - 1][colIndex].isFixed == false) {
                board[row - 1][colIndex].asterisk = true;
            }
        }
    }
    return valExist;
}

bool val_in_column(cell **board, int column, int row, int val) {
    /*
     * Checks if value exist in the given column
     */
    int N = blockRows * blockCols;
    bool valExist = false;
    int rowIndex;
    for (rowIndex = 0; rowIndex < N; rowIndex++) {
        if(rowIndex == row - 1){/*not checking cell to be changed*/}
        else if (board[rowIndex][column - 1].number == val) {
            valExist = true;
            if (board[rowIndex][column - 1].isFixed == false) {
                board[rowIndex][column - 1].asterisk = true;
            }
        }
    }
    return valExist;
}

bool valid_check(cell **board, int column, int row, int val) {
    /*
     * Checks if validation of given value in cell <row,column> according to sudoku rules
     * marks aserisks
     */

    if(val_in_block(board, column, row, val) | val_in_row(board, column, row, val) | val_in_column(board, column, row, val)){
        board[row - 1][column - 1].asterisk = true;
        return false;
    }
    else{
        board[row - 1][column - 1].asterisk = false;
        return true;
    }

}

void game_over(cell **board){
    /*
     * Checks if board is full and solved correctly
     */
    int col, row;
    bool solvable;
    bool full = true;
    cell** copyBoard;
    int N = blockCols * blockRows;
    if (check_board_erroneous(board)){
        return;
    }
    for (col = 0; col < N; col++) {
        for (row = 0; row < N; row++) {
            if (board[row][col].number == UNASSIGNED) {
                full = false;
            }
        }
    }
    if (full) {
        copyBoard = generate_empty_board();
        solvable = ILP(board, copyBoard);
        free_board(copyBoard);
        if (solvable) {
            printf(GAME_OVER);
            mode = 'I';
            gameOver = true;
        } else {
            printf("Puzzle solution erroneous\n");
        }
    }
}

bool auto_fill(cell **board)	{
    /*
     * Autofills cells which contain a single legal value
     */
    int i, j, k, candidate;
    int fillFlag = false;
    int numOfCandidates = 0;
    int maxValue = blockRows * blockCols + 1;
    cell **copyOfBoard;
    int N = blockRows * blockCols;
    if (check_board_erroneous(board)){
        printf("Error: board contains erroneous values\n");
        return false;
    }
    copyOfBoard = generate_empty_board();
    copy_board(board, copyOfBoard);
    for (i = 0; i < N; i++){
        for (j = 0; j < N; j++){
            if (copyOfBoard[i][j].number == UNASSIGNED){
                for (k = 1; k < maxValue; k++){
                    if (valid_check(copyOfBoard, j + 1, i + 1, k)){
                        candidate = k;
                        numOfCandidates++;
                    }
                }
                if (numOfCandidates == 1){
                    board[i][j].number = candidate;
                    validate_risks(board, j + 1, i + 1);
                    fillFlag = true;
                    printf("Cell <%d,%d> set to %d\n", (j+1), (i+1), candidate);
                }
                numOfCandidates = 0;
            }

        }
    }
    free_board(copyOfBoard);
    print_board(board, 'E');
    game_over(board);
    return fillFlag;
}

bool validate_risks(cell **board, int column, int row) {
    int initialCol, initialRow, colIndex, rowIndex;
    int N = blockRows * blockCols;
    bool risksFound = true;
    initialCol = get_block_col_index(column);
    initialRow = get_block_row_index(row);
    for (colIndex = initialCol; (colIndex < blockCols + initialCol); colIndex++) { /*block*/
        for (rowIndex = initialRow; (rowIndex < blockRows + initialRow); rowIndex++) {
            if (colIndex == (column-1) && rowIndex == (row-1)) {
                /*not checking cell to be changed*/
            }
            else if (board[rowIndex][colIndex].asterisk){
                if(valid_check(board, colIndex + 1, rowIndex + 1, board[rowIndex][colIndex].number)){
                    board[rowIndex][colIndex].asterisk = false;
                }
                else{
                    risksFound = false;
                }
            }
        }
    }
    for (colIndex = 0; (colIndex < N); colIndex++) { /*row*/
        if (colIndex == column - 1) {
        }
        else if (board[row - 1][colIndex].asterisk){
            if (valid_check(board, colIndex + 1, row, board[row - 1][colIndex].number)){
                board[row - 1][colIndex].asterisk = false;
            }
            else{
                risksFound = false;
            }
        }
    }

    for (rowIndex = 0; (rowIndex < N); rowIndex++) {  /*col*/
        if (rowIndex == row - 1){
        }
        else if (board[rowIndex][column - 1].asterisk){
            if( valid_check(board, column , rowIndex + 1, board[rowIndex][column - 1].number)){
                board[rowIndex][column - 1].asterisk = false;
            }
            else{
                risksFound = false;
            }
        }
    }
    return risksFound;
}

bool set(cell **board, int column, int row, int val, char mode) {
    int N = blockRows * blockCols;
    if((!valid_board_index(column, N)) || (!valid_board_index(row, N)) || (!valid_set_value(val, N))){
        printf(VALUE_RANGE_ERROR, blockCols * blockRows);
        return false;
    }
    if (board[row - 1][column - 1].isFixed) {
        printf(FIXED_ERROR);
        return false;
    }
    else if (board[row - 1][column - 1].number == val) {
        print_board(board, mode);
        return  false;
    }
    else if (val == 0) {
        board[row - 1][column - 1].number = UNASSIGNED;
        board[row - 1][column - 1].asterisk = false;
        validate_risks(board, column, row);
        print_board(board, mode);
        return true;
    }
    else {
        valid_check(board, column, row, val); /*make valid check to updated astrisk*/
        board[row - 1][column - 1].number = val;
        validate_risks(board, column, row);
        print_board(board, mode);
        if (mode == 'S'){
            game_over(board);
        }

        return true;
    }
}

bool check_board_erroneous(cell **board){
    int colIndex, rowIndex = 0;
    int N = blockRows * blockCols;
    for (rowIndex = 0; rowIndex < N; rowIndex++) {
        for (colIndex = 0; colIndex < N; colIndex++) {
            if (board[rowIndex][colIndex].asterisk) {
                return true;
            }
        }
    }
    return false;
}

void hint(cell **board, int column, int row){
    /*
     * Prints the value of the cell <row,column> on the last solved sudoku board
     */
    int hint;
    bool solvable;
    cell **solvedBoard = NULL;
    if (check_board_erroneous(board)) {
        printf("Error: board contains erroneous values\n");
        return;
    }
    if (board[row - 1][column - 1].isFixed) {
        printf(FIXED_ERROR);
        return;
    }
    if (board[row - 1][column - 1].number != 0) {
        printf("Error: cell already contains a value\n");
        return;
    }
    solvedBoard = generate_empty_board();
    solvable = ILP(board,solvedBoard);
    if (solvable == false){
        printf("Error: board is unsolvable\n");
        return;
    }
    else{
         hint = solvedBoard[row - 1][column - 1].number;
         printf("Hint: set cell to %d\n", hint);
         free_board(solvedBoard);
        }
}

void free_board(cell** board){
    int i;
    int N = blockRows * blockCols;
    for (i = 0; i < N; i++) {
        free(board[i]);
    }
    free(board);
    board = NULL;

}

void exit_game(char* command){
    /*
     * Free up memeory and exists the program
     */
    printf("Exiting...\n");
    free(command);
    exit(0);
}

bool fill_cell(cell** board, int column, int row){
    /*fills the cell with legal random legal value, if no value found returns false*/
    cell ** copyOfBoard;
    int numbersLeft, randomIndex, nextNum;
    int N = blockRows * blockCols;
    int* availableNumbers;
    availableNumbers = generate_int_array(N);
    copyOfBoard = generate_empty_board();
    copy_board(board, copyOfBoard);
    numbersLeft = N;
    while (numbersLeft > 0){
        randomIndex = rand() % numbersLeft;
        nextNum = availableNumbers[randomIndex];
        if(valid_check(copyOfBoard, column + 1 ,row + 1, nextNum)){
            copyOfBoard[row][column].number = nextNum;
            copy_board(copyOfBoard, board);
            free_board(copyOfBoard);
            free(availableNumbers);
            return true;
        }
        copyOfBoard[row][column].number = UNASSIGNED;
        copyOfBoard[row][column].asterisk = false;
        numbersLeft--;
        if (numbersLeft > 0) {
            delFromArr(randomIndex, numbersLeft, availableNumbers);
        }
}
    free_board(copyOfBoard);
    free(availableNumbers);
    return false;
}

bool generate_randomized_solved_board (cell** board, int initialFullCells, int copyCells) {
    cell **newBoard;
    bool solvable;
    int randCol, randRow, emptyCellsCounter;
    int cellsFilled = 0;
    int triesLeft = 1000;
    int N = blockCols * blockRows;
    emptyCellsCounter = count_empty_cells(board);
    if ((!valid_set_value(initialFullCells, emptyCellsCounter)) || (!valid_set_value(copyCells, emptyCellsCounter))) {
        printf(VALUE_RANGE_ERROR, emptyCellsCounter);
        return false;
    }
    if (!board_is_empty(board)) {
        printf("Error: board is not empty\n");
        return false;
    }
    while (triesLeft > 0) {

        if (cellsFilled < initialFullCells) {
            randCol = rand() % N;
            randRow = rand() % N;
            if (board[randRow][randCol].number == UNASSIGNED) {
                if (!fill_cell(board, randCol, randRow)) {
                    triesLeft--;
                    empty_board(board);
                    cellsFilled = 0;
                } else {
                    cellsFilled++;
                }
            }
        }
        else if (cellsFilled == initialFullCells) {
            newBoard = generate_empty_board();
            solvable = ILP(board, newBoard);
            empty_board(board);
            if (solvable) {
                copy_random_cells(newBoard, copyCells, board);
                free_board(newBoard);
                print_board(board, 'E');
                return true;
            } else {
                triesLeft--;
                empty_board(board);
                empty_board(newBoard);
                cellsFilled = 0;
            }
        }
    }
    printf("Error: puzzle generator failed\n");
    return false;
}