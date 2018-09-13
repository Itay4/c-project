#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "main_aux.h"
#include "game.h"
#include "stack.h"
#include <string.h>
#include "gurobi_c.h"

typedef struct validPlays {
    int* validPlaysArray;
    int numOfPlays;
} validPlays;

bool recursive_backtrack(cell** board, int row, int column) {
    /*
     * Recursive backtrack to solve sudoku board
     */
    int nextNum, i, j;
    int randomIndex;
    int availableNumbers[9];
    j = 0;
    if (row == 9) {
        return true;
    }

    /* If cell number is already set, no need to change and recruse to next cell */
    if (board[row][column].number) {
        if (column == 8) {
            if (recursive_backtrack(board, row+1, 0)) return true;
        } else {
            if (recursive_backtrack(board, row, column+1)) return true;
        }
        return false;
    }
    for (i = 1; i < 10; i++){
        if (valid_check(board, column, row, i)) {
            availableNumbers[j] = i;
            j++;}
    }
    while (j > 0) {
        if (j==1) { 
            nextNum = availableNumbers[0];
        } else {
            randomIndex = rand() % j;
            nextNum = availableNumbers[randomIndex];
        }
        board[row][column].number = nextNum;
        del_from_arr(randomIndex + 1, j, availableNumbers);
        j--;
        if (column == 8) {
            if (recursive_backtrack(board, row + 1, 0)) return true;
        } else {
            if (recursive_backtrack(board, row, column + 1)) return true;
        }
        board[row][column].number = 0;

    }
    return false;
}

int get_actual_value(cell** board, int i, int j) {
    /* Gets the actual value of cell */
    int value = board[i][j].number;
    int N = rows*cols;
    if (value < 0) {
        value = value * (-1);
    }
    if (value > N) { 
        value = value - N;
    }
    return value;
}

validPlays* get_valid_plays(cell** board, int i, int j) {

    int* plays;
    int* legalPlays;
    int counter, k, l, roundI, roundJ, value, N;
    validPlays* result;

    N = rows*cols;
    counter = 0;
    plays = calloc(N+1, sizeof(int));
    for (k = 0; k < N; k++) {
        value = get_actual_value(board, k, j);
        if (value != 0) {
            plays[value] = 1;
        }
    }
    for (k = 0; k < N; k++) {
        value = get_actual_value(board, i, k);
        if (value != 0) {
            plays[value] = 1;
        }
    }

    roundI = i/rows;
    roundJ = j/cols;
    for (k = rows*roundI; k < rows*(roundI)+rows; k++){
        for(l = cols*roundJ; l<cols+cols*roundJ; l++) {
            value = get_actual_value(board, k, l);
            if (value != 0) {
            plays[value] = 1;
            }
        }
    }
    for (k = 1; k < N+1; k++) {
        if (plays[k] == 0) {
            counter++;
        }
    }
    legalPlays = calloc(counter, sizeof(int));
    l = 0;
    for (k = 1; k < N+1; k++) {
        if (plays[k] == 0) {
            legalPlays[l] = k;
            l++;
        }
    }
    result = (validPlays*) malloc(sizeof(validPlays));
    result->validPlaysArray = legalPlays;
    result->numOfPlays = counter;
    free(plays);
    return result;
}

cell** duplicate_board(cell** oldBoard) {
    /* Duplicates sudoku board */
    cell** newBoard;
    int N, i, j;
    N = rows * cols;
    newBoard=generateEmptyBoard();
    for (i = 0; i < N; i++){
        for (j = 0; j < N; j++) {
            newBoard[i][j].number = oldBoard[i][j].number;
        }
    }
    return newBoard;
}

int deterministic_backtrack(cell** board, int i, int j) {
    /* Counts number of solutions using exhaustive deterministic backtrack */
    element* e;
    int counter, firstPlay, N, k, nextI, nextJ, newI, newJ;
    int data[2];
    int* auxData;

    validPlays* legalPlays;
    cell** auxBoard;
    cell** finalBoard;
    stack* stck=calloc(1,sizeof(stack));
    N = rows * cols;

    stack_initialize(stck);
    data[0]=i;
    data[1]=j;
    push(data, board, stck);

    while (stck->counter > 0){
        e = pop(stck);
        auxData = e->data;
        if (firstPlay == 0){
            firstPlay = 1;
            auxBoard = duplicate_board(board);
            nextI = data[0];
            nextJ = data[1];
            newI = data[0];
            newJ = data[1];
        } else {
            auxBoard = duplicate_board(e->board);
            nextI = auxData[0];
            nextJ = auxData[1];
            newI = auxData[0];
            newJ = auxData[1];
        }
        legalPlays = (validPlays*) get_valid_plays(auxBoard, newI, newJ);
        do{
            nextJ = (nextJ+1) % N;
            if (nextJ == 0){
                nextI = (nextI+1) % N;
            }
        }
        while((nextJ != N-1 || nextI != N-1) && auxBoard[nextI][nextJ].number != 0);

        if((((newI == N-1 && newJ == N-1)|| (nextI == N-1 && nextJ == N-1 && auxBoard[nextI][nextJ].number!=0)) && ((legalPlays->numOfPlays) == 1))) {
                counter++;
                continue;
            } else {
            for (k = 0; k < (legalPlays->numOfPlays); k++){
                auxData = calloc(2,sizeof(int));
                finalBoard = duplicate_board(auxBoard);
                finalBoard[newI][newJ].number = (legalPlays->validPlaysArray)[k];
                auxData[0] = nextI;
                auxData[1] = nextJ;
                push(auxData, finalBoard, stck);
            }

            free(legalPlays->validPlaysArray);
            free(legalPlays);
            }
        }
    free(stck);
    return counter;
}

void set_fixed_cells(cell **board, int fixedCells) {
    /*
     * Randomly chooses given number of cells in the game board and sets them as fixed
     */
    int i, randX, randY;
    for (i = 0; i < fixedCells; i++){
        while (true) {
            randX = rand() % NUM_OF_ROWS;
            randY = rand() % NUM_OF_COLUMNS;
            if (board[randY][randX].isFixed != true){
                board[randY][randX].isFixed = true;
                break;
            }
        }

    }
}

void generate_solved_board(cell** board, int fixedCells){
    /*
     * Generates solved sudoku game board
     */
    recursive_backtrack(board, 0, 0);
    set_fixed_cells(board, fixedCells);
}

/*void generate_user_board(cell **solved_board, cell** user_board) {

     * Generates unsolved user sudoku game board

    int i,j;
    for (i=0; i < NUM_OF_ROWS; i++) {
        for (j=0; j < NUM_OF_COLUMNS; j++) {
            if (solved_board[i][j].isFixed) {
                user_board[i][j].number = solved_board[i][j].number;
                user_board[i][j].isFixed = true;
            }
            else{
                user_board[i][j].number = UNASSIGNED;
                user_board[i][j].isFixed = false;
            }
        }
    }
    printBoard(user_board);
}*/

void send_error(int error, char* str, GRBenv* env){
    printf("ERROR %d %s: %s\n", error, str, GRBgeterrormsg(env));
}


cell** prepare_board_for_gurobi(cell** board) {
    /* Prepares board for Gurobi by fixing its values */
    int i, j, N;
    cell** resultBoard= generateEmptyBoard();
    N = rows * cols;
    for (i = 0; i < N; i++){
        for (j = 0; j < N; j++){
            resultBoard[i][j].number = get_actual_value(board, i, j);
        }
    }
    return resultBoard;
}

int ILP(cell **board, cell **solvedBoard) {

    cell **auxBoard; 
    int *ind, n, m, N, error, count, i, j, p, t, v, optimstatus;
    GRBenv   *env   = NULL;
    GRBmodel *model = NULL;
    double *lb, *val, objval, *sol;
    char *vtype, **names, *namestorage, *cursor;

    n = rows;
    m = cols;
    N = rows * cols;
    error = 0;
    auxBoard = prepare_board_for_gurobi(board);
    names = (char**) calloc(N*N*N, sizeof(char*));
    namestorage = (char*) calloc(20*N*N*N, sizeof(char));
    ind = (int*) calloc(N*N*N, sizeof(int));
    val = (double*) calloc(N*N*N, sizeof(double));
    lb = (double*) calloc(N*N*N, sizeof(double));
    vtype = (char*) calloc(N*N*N, sizeof(char));
    sol = (double*) calloc(N*N*N, sizeof(double));
    
    /* Create an empty model */

    cursor = namestorage;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (v = 0; v < N; v++) {
                if(auxBoard[i][j].number == (v+1)){
                    lb[i*N*N+j*N+v] = 1;
                } else {
                    lb[i*N*N+j*N+v] = 0;
                }
                vtype[i*N*N+j*N+v] = GRB_BINARY;
                names[i*N*N+j*N+v] = cursor;
                sprintf(names[i*N*N+j*N+v], "x[%d,%d,%d]", i, j, v+1);
                cursor += strlen(names[i*N*N+j*N+v]) + 1;
            }
        }
    }

    /* Create environment */

    error = GRBloadenv(&env, "sudoku.log");
    
    if (error) {
        #ifdef PRINT_GUROBI_ERRORS
        send_error(error, "GRBloadenv", env); 
        #endif
        return 0;
        }
    
    /* Removes Gurobi prints */
    
    error = GRBsetintparam(env, "OutputFlag", 0);
    if (error) {
        #ifdef PRINT_GUROBI_ERRORS
        send_error(error, "GRBsetintparam or GRBgetenv", env);
        #endif
        return 0;
    }
  
    /* Create new model */

    error = GRBnewmodel(env, &model, "sudoku", N*N*N, NULL, lb, NULL, vtype, names);
    if (error) {
        #ifdef PRINT_GUROBI_ERRORS
        send_error(error, "GRBnewmodel", env);
        #endif
        return 0;
    }
    
    
    /* Each cell gets a value */

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (v = 0; v < N; v++) {
                ind[v] = i*N*N + j*N + v;
                val[v] = 1.0;
            }
            
            error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) {
                #ifdef PRINT_GUROBI_ERRORS
                send_error(error, "GRBaddconstr1", env); 
                #endif
                return 0;
            }
        }
    }

    /* Each value must appear once in each row */

    for (v = 0; v < N; v++) {
        for (j = 0; j < N; j++) {
            for (i = 0; i < N; i++) {
                ind[i] = i*N*N + j*N + v;
                val[i] = 1.0;
            }
            
            error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) {
                #ifdef PRINT_GUROBI_ERRORS
                send_error(error, "GRBaddconstr2", env);
                #endif
                return 0;
            }
        }
    }

    /* Each value must appear once in each column */

    for (v = 0; v < N; v++) {
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                ind[j] = i*N*N + j*N + v;
                val[j] = 1.0;
            }

            error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) {
                #ifdef PRINT_GUROBI_ERRORS
                send_error(error, "GRBaddconstr3", env);
                #endif
                return 0;
            }
        }
    }

    /* Each value must appear once in each subgrid */

    for (v = 0; v < N; v++) {
        for (p = 0; p < m; p++) {
            for (t = 0; t < n; t++) {
                count = 0;
                for (i = p*n; i < (p+1)*n; i++) {
                    for (j = t*m; j < (t+1)*m; j++) {
                        ind[count] = i*N*N + j*N + v;
                        val[count] = 1.0;
                        count++;
                    }
                }
                
                error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
                if (error) {
                    #ifdef PRINT_GUROBI_ERRORS
                    send_error(error, "GRBaddconstr4", env);
                    #endif
                    return 0;
                }
            }
        }
    }
    
    /* Optimize model */

    error = GRBoptimize(model);
    if (error) {
        #ifdef PRINT_GUROBI_ERRORS
        send_error(error, "GRBoptimize", env);
        #endif
        return 0;
    }   

    /* Capture solution information */

    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error) {
        #ifdef PRINT_GUROBI_ERRORS
        send_error(error, "GRBgetintattr", env);
        #endif
        return 0;
    }
    
    error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
    if (error) {
        #ifdef PRINT_GUROBI_ERRORS
        send_error(error, "GRBgetdblattr", env);
        #endif
        return 0;
    }
    
    
    error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, N*N*N, sol);   
    if (error) {
        #ifdef PRINT_GUROBI_ERRORS
        send_error(error, "GRBgetdblattrarray", env);
        #endif
        return 0;
    }
    
    #ifdef PRINT_GUROBI_ERRORS
    printf("\nOptimization complete\n");
    #endif

    if (optimstatus == GRB_OPTIMAL) {
        #ifdef PRINT_GUROBI_ERRORS
        printf("Optimal objective function: %.4e\n", objval);       
        #endif
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                for (v = 0; v < N; v++) {
                    if (sol[i*N*N + j*N + v] != 0) {     
                        solvedBoard[i][j].number = v+1;
                    }
                }
            }
        }
    } else {
        if(optimstatus == GRB_INF_OR_UNBD){
            #ifdef PRINT_GUROBI_ERRORS
            printf("Model is infeasible or unbounded\n");
            #endif
        } else {
            #ifdef PRINT_GUROBI_ERRORS
            printf("Optimization was stopped early\n");
            #endif
        }
    }

    /* Free Resources */
    free(ind); 
    free(val); 
    free(lb); 
    free(vtype); 
    free(sol); 
    free(names); 
    free(namestorage);
    GRBfreemodel(model);
    GRBfreeenv(env);
    return 1;
}