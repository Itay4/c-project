/** @file solve.c
 *  @brief solve source file.
 *
 *  This module implements different methods to solve sudoku game.
 *  Includes functions required to solve sudoku board.
 *
 *  @author Itay Keren (itaykeren)
 *  @author Rotem Bar (rotembar)
 *
 */

/* -- Includes -- */

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


/* Returns the actual value of a cell in the sudoku board */
int get_actual_value(cell** board, int i, int j) {
    int value = board[i][j].number;
    int N = blockRows * blockCols;
    if (value < 0) {
        value = value * (-1);
    }
    if (value > N) {
        value = value - N;
    }
    return value;
}

/* Returns the number of valid moves from given indices based on given sudoku board */
validPlays* get_valid_plays(cell** board, int i, int j) {
    int* plays;
    int* legalPlays;
    int counter, k, l, roundI, roundJ, value, N;
    validPlays* result;
    N = blockRows * blockCols;
    counter = 0;
    plays = calloc(N+1, sizeof(int));
    if (plays == NULL) {
        memory_error("Memory allocation failed\n");
        exit(0);
    }
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
    roundI = i/blockRows;
    roundJ = j/blockCols;
    for (k = blockRows*roundI; k < blockRows*(roundI)+blockRows; k++){
        for(l = blockCols*roundJ; l<blockCols+blockCols*roundJ; l++) {
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
    if (legalPlays == NULL) {
        memory_error("Memory allocation failed\n");
        exit(0);
    }
    l = 0;
    for (k = 1; k < N+1; k++) {
        if (plays[k] == 0) {
            legalPlays[l] = k;
            l++;
        }
    }
    result = (validPlays*) malloc(sizeof(validPlays));
    if (result == NULL) {
        memory_error("Memory allocation failed\n");
        exit(0);
    }
    result->validPlaysArray = legalPlays;
    result->numOfPlays = counter;
    free(plays);
    return result;
}

/* Duplicates sudoku board */
cell** duplicate_board(cell** oldBoard) {
    /* Duplicates sudoku board */
    cell** newBoard;
    newBoard = generate_empty_board();
    copy_board(oldBoard, newBoard);
    return newBoard;
}

/* Counts number of solutions using exhaustive deterministic backtrack based on recurive stack */
int deterministic_backtrack(cell** board, int i, int j) {
    element* e;
    int counter=0, firstPlay=0, N, k, nextI, nextJ, newI, newJ;
    int data[2];
    int* auxData;
    validPlays* legalPlays;
    cell **auxBoard, **finalBoard;
    stack* stck = calloc(1,sizeof(stack));
    if (stck == NULL) {
        memory_error("Memory allocation failed\n");
        exit(0);
    }
    N = blockRows * blockCols;
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
            free(e);
        } else {
            auxBoard = duplicate_board(e->board);
            nextI = auxData[0];
            nextJ = auxData[1];
            newI = auxData[0];
            newJ = auxData[1];
            free_board(e->board);
            free(auxData);
            free(e);
        }
        legalPlays = (validPlays*) get_valid_plays(auxBoard, newI, newJ);
        do {
            nextJ = (nextJ+1) % N;
            if (nextJ == 0){
                nextI = (nextI+1) % N;
            }
        }
        while((nextJ != N-1 || nextI != N-1) && auxBoard[nextI][nextJ].number != 0);
        if((((newI == N-1 && newJ == N-1)|| (nextI == N-1 && nextJ == N-1 && auxBoard[nextI][nextJ].number!=0)) && ((legalPlays->numOfPlays) == 1))) {
            free(legalPlays->validPlaysArray);
            free(legalPlays);
            counter++;
            free_board(auxBoard);
            continue;
        } else {
            for (k = 0; k < (legalPlays->numOfPlays); k++){
                auxData = calloc(2,sizeof(int));
                if (auxData == NULL) {
                    memory_error("Memory allocation failed\n");
                    exit(0);
                }
                finalBoard = duplicate_board(auxBoard);
                finalBoard[newI][newJ].number = (legalPlays->validPlaysArray)[k];
                auxData[0] = nextI;
                auxData[1] = nextJ;
                push(auxData, finalBoard, stck);
            }
            free(legalPlays->validPlaysArray);
            free(legalPlays);
        }
        free_board(auxBoard);
    }

    free(stck);
    return counter;
}

/* Randomly chooses given number of cells and unassign them. */
void copy_random_cells(cell **solvedBoard, int copyCells, cell** finalBoard) {
    
    int i, colsIndex, rowsIndex;
    int N = blockCols * blockRows;
    for (i = 0; i < copyCells; i++){
        while (true) {
            colsIndex = rand() % N;
            rowsIndex = rand() % N;
            if (finalBoard[rowsIndex][colsIndex].number == UNASSIGNED){
                finalBoard[rowsIndex][colsIndex].number = solvedBoard[rowsIndex][colsIndex].number;
                break;
            }
        }
    }
}

/* Prints errors raised by Gurobi module */
void send_error(int error, char* str, GRBenv* env){
    printf("ERROR %d %s: %s\n", error, str, GRBgeterrormsg(env));
}

/* Prepares board for Gurobi by fixing its values */
cell** prepare_board_for_gurobi(cell** board) {
    int i, j, N;
    cell** resultBoard= generate_empty_board();
    N = blockRows * blockCols;
    for (i = 0; i < N; i++){
        for (j = 0; j < N; j++){
            resultBoard[i][j].number = get_actual_value(board, i, j);
        }
    }
    return resultBoard;
}

/* Checks if sudoku board is feasible by ILP algorithm using Gurobi */
bool ILP(cell **board, cell **solvedBoard) {
    cell **auxBoard;
    int *ind, n, m, N, error, count, i, j, p, t, v, optimstatus;
    GRBenv   *env   = NULL;
    GRBmodel *model = NULL;
    double *lb, *val, objval, *sol;
    char *vtype, **names, *namestorage, *cursor;
    n = blockRows;
    m = blockCols;
    N = n * m;
    error = 0;
    auxBoard = prepare_board_for_gurobi(board);
    names = (char**) calloc(N*N*N, sizeof(char*));
    if (names == NULL) {
        memory_error("Memory allocation failed\n");
        exit(0);
    }
    namestorage = (char*) calloc(20*N*N*N, sizeof(char));
    if (namestorage == NULL) {
        memory_error("Memory allocation failed\n");
        exit(0);
    }
    ind = (int*) calloc(N*N*N, sizeof(int));
    if (ind == NULL) {
        memory_error("Memory allocation failed\n");
        exit(0);
    }
    val = (double*) calloc(N*N*N, sizeof(double));
    if (val == NULL) {
        memory_error("Memory allocation failed\n");
        exit(0);
    }
    lb = (double*) calloc(N*N*N, sizeof(double));
    if (lb == NULL) {
        memory_error("Memory allocation failed\n");
        exit(0);
    }
    vtype = (char*) calloc(N*N*N, sizeof(char));
    if (vtype == NULL) {
        memory_error("Memory allocation failed\n");
        exit(0);
    }
    sol = (double*) calloc(N*N*N, sizeof(double));
    if (sol == NULL) {
        memory_error("Memory allocation failed\n");
        exit(0);
    }
    /* Create an empty model */
    cursor = namestorage;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (v = 0; v < N; v++) {
                if (auxBoard[i][j].number == (v+1)){
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
        send_error(error, "GRBloadenv", env);
        return false;
    }
    /* Removes Gurobi prints */
    error = GRBsetintparam(env, "OutputFlag", 0);
    if (error) {
        send_error(error, "GRBsetintparam or GRBgetenv", env);
        return false;
    }
    /* Create new model */
    error = GRBnewmodel(env, &model, "sudoku", N*N*N, NULL, lb, NULL, vtype, names);
    if (error) {
        send_error(error, "GRBnewmodel", env);
        return true;
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
                send_error(error, "GRBaddconstr1", env);
                return false;
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
                send_error(error, "GRBaddconstr2", env);
                return false;
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
                send_error(error, "GRBaddconstr3", env);
                return false;
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
                    send_error(error, "GRBaddconstr4", env);
                    return false;
                }
            }
        }
    }
    /* Optimize model */
    error = GRBoptimize(model);
    if (error) {
        send_error(error, "GRBoptimize", env);
        return false;
    }
    /* Capture solution information */
    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error) {
        send_error(error, "GRBgetintattr", env);
        return false;
    }
    error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
    if (error) {
        send_error(error, "GRBgetdblattr", env);
        return false;
    }
    error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, N*N*N, sol);
    if (error) {
        send_error(error, "GRBgetdblattrarray", env);
        return false;
    }
    if (optimstatus == GRB_OPTIMAL) {
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
        if (optimstatus == GRB_INF_OR_UNBD) {
            printf("Model is infeasible or unbounded\n");
        } else {
            printf("Optimization was stopped early\n");
        }
    }
    /* Free Resources */
    free_board(auxBoard);
    free(ind);
    free(val);
    free(lb);
    free(vtype);
    free(sol);
    free(names);
    free(namestorage);
    GRBfreemodel(model);
    GRBfreeenv(env);
    return true;
}