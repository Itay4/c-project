
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"
#include "main_aux.h"
#include "game.h"
#include "parser.h"


node* create_node(cell** board) {
    node* newNode = malloc(sizeof(node));
    if (newNode == NULL) {
        memory_error("Memory allocation failed\n");
        exit(0);
    }
    newNode->board = board;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

list* create_list(cell** board) {
    list* lst = malloc(sizeof(list));
    if (lst == NULL) {
        memory_error("Memory allocation failed\n");
        exit(0);
    }
    lst->head = create_node(board);
    lst->tail = NULL;
    lst->current = lst->head;
    return lst;
}

void delete_next_nodes(node* newTail, list* lst) {
    node* currentToDel;
    if (newTail == NULL){
        return;
    }
    currentToDel = lst->current->next;
    newTail->next = NULL;
    newTail->prev = lst->current;
    lst->tail = newTail;
    lst->current->next = newTail;
    lst->current = newTail;
    if (currentToDel == NULL) {
        return; /*no nodes to delete*/
    } else {
        while (currentToDel->next != NULL) {
            currentToDel = currentToDel->next;
            free_board(currentToDel->prev->board);
            free(currentToDel->prev);
        }
    }
    free_board(currentToDel->board);
    free(currentToDel);
}

void insert_at_tail(cell** board, list* lst) {
    node* temp = lst->current;
    node* newNode = create_node(board);
    if (temp == lst->head) { /*first move*/
        lst->head->next = newNode;
        newNode->prev = lst->head;
        lst->current = newNode;
        lst->tail = newNode;
        return;
    } else if (temp != lst->tail){
        delete_next_nodes(newNode, lst);
    } else {
        lst->tail->next = newNode;
        newNode->prev = lst->tail;
        lst->tail = newNode;
        lst->current = newNode;
    }
}

void free_list(list* lst) {
    node* currentToDel = lst->head;
    while (currentToDel->next != NULL) {
        currentToDel = currentToDel->next;
        free_board(currentToDel->prev->board);
        free(currentToDel->prev);
    }
    free_board(currentToDel->board);
    free(currentToDel);
    free(lst);
    lst = NULL;
}

void print_board_changes(cell** oldBoard, cell** newBoard, char* cmdType) {
    int i, j, oldVal, newVal;
    int N = blockRows * blockCols;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            oldVal = oldBoard[i][j].number;
            newVal = newBoard[i][j].number;
            if (oldVal != newVal) {
                if (oldVal == UNASSIGNED) {
                    printf("%s %d,%d: from _ to %d\n",cmdType, j + 1, i + 1, newVal);
                } else if (newVal == UNASSIGNED) {
                    printf("%s %d,%d: from %d to _\n",cmdType, j + 1, i + 1, oldVal);
                } else {
                    printf("%s %d,%d: from %d to %d\n",cmdType, j + 1, i + 1, oldVal, newVal);
                }
            }
        }
    }
}

void redo(list * lst, cell **board, char mode){
    node * newCurrent;
    if ((lst->current == lst->tail) || (lst->head->next == NULL)) {
        printf("Error: no moves to redo\n");
        return;
    }
    newCurrent = lst->current->next;
    copy_board(newCurrent->board, board);
    print_board(board, mode);
    print_board_changes(lst->current->board, newCurrent->board, "Redo");
    lst->current = newCurrent;
}

void undo(list * lst, cell **board, char mode){
    node * newCurrent;
    if ((lst->current == lst->head) || (lst->head->next == NULL)){
        printf("Error: no moves to undo\n");
        return;
    }
    newCurrent = lst->current->prev;
    copy_board(newCurrent->board, board);
    print_board(board, mode);
    print_board_changes(lst->current->board, newCurrent->board, "Undo");
    lst->current = newCurrent;
}

void reset(list * lst, cell **board, char mode){
    delete_next_nodes(lst->head, lst);
    copy_board(lst->current->board, board);
    print_board(board, mode);
    printf("Board reset\n");

}