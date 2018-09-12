
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"
#include "main_aux.h"
#include "game.h"
#include "parser.h"

node* create_node(cell **board) {
    node* newNode = malloc(sizeof(node));
    newNode->board = board;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

list* create_list(cell **board){
    list * list = malloc(sizeof(list));
    list->head = create_node(board);
    list->tail = NULL;
    list->current = NULL;
    return list;
}

void delete_next_nodes(node* newTail, list *lst) {
    node * currentToDel = newTail->next;
    newTail->next = NULL;
    lst->tail = newTail;
    lst->current = newTail;
    if (currentToDel == NULL) {
        return; /*no nodes to delete*/
    }
    else{
        while (currentToDel->next != NULL) {
            currentToDel = currentToDel->next;
            free_board(currentToDel->prev->board);
            free(currentToDel->prev);
            }
        }
        free_board(currentToDel->board);
        free(currentToDel);
    }

void insert_at_tail(cell **board, list *lst) {
    node *temp = lst->current;
    node *newNode = create_node(board);
    if (temp == NULL) { /*first move*/
        lst->head->next = newNode;
        newNode->prev = lst->head;
        lst->current = newNode;
        lst->tail = newNode;
        return;
    }
    else if (temp != lst->tail){
        delete_next_nodes(temp, lst);
    }
    else {
        lst->tail->next = newNode;
        newNode->prev = lst->tail;
        lst->tail = newNode;
        lst->current = newNode;
    }
}

void free_list(list * lst){
    node * current = lst->head;
    delete_next_nodes(current, lst);
    free_board(lst->current->board);
    free(current);
    free(lst);
    lst = NULL;
}

void print_board_changes(cell ** oldBoard, cell ** newBoard) {
    int i, j, oldVal, newVal;
    int N = rows * cols;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            oldVal = oldBoard[i][j].number;
            newVal = newBoard[i][j].number;
            if (oldVal != newVal) {
                if (oldVal == UNASSIGNED) {
                    printf("Redo %d,%d: from - to %d\n", j + 1, i + 1, newVal);
                } else if (newVal == UNASSIGNED) {
                    printf("Redo %d,%d: from %d to -\n", j + 1, i + 1, oldVal);
                } else {
                    printf("Undo %d,%d: from %d to %d\n", j + 1, i + 1, oldVal, newVal);
                }
            }
        }
    }
}

void redo(list * lst, cell **board){
    node * newCurrent;
    if ((lst->current == lst->tail) || (lst->head->next == NULL)) {
        printf("Error: no moves to redo\n");
        return;
    }
    newCurrent = lst->current->next;
    copy_board(newCurrent->board, board);
    print_board(board);
    print_board_changes(lst->current->board, newCurrent->board);
    lst->current = newCurrent;
}

void undo(list * lst, cell **board){
    node * newCurrent;
    if ((lst->current == lst->head) || (lst->head->next == NULL)){
        printf("Error: no moves to undo\n");
        return;
    }
    newCurrent = lst->current->prev;
    copy_board(newCurrent->board, board);
    print_board(board);
    print_board_changes(lst->current->board, newCurrent->board);
    lst->current = newCurrent;
}

void reset(list * lst, cell **board){
    delete_next_nodes(lst->head, lst);
    copy_board(lst->current->board, board);
    print_board(board);
    printf("Board reset\n");

}


