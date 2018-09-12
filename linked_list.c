
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"
#include "main_aux.h"
#include "game.h"
#include "parser.h"



node* CreateNode(cell **board) {
    node* new_node = malloc(sizeof(node));
    new_node->board = board;
    new_node->prev = NULL;
    new_node->next = NULL;
    return new_node;
}
/*maybe add function of create data- reset board and then copy new board to it*/
list* CreateList(cell **board){
    list * list = malloc(sizeof(list));
    list->head = CreateNode(board);
    list->tail = NULL;
    list->current = NULL;
    return list;
}

void DeleteNextNodes(node* new_tail, list *lst) {
    node * current_to_del = new_tail->next;
    new_tail->next = NULL;
    lst->tail = new_tail;
    lst->current = new_tail;
    if (current_to_del == NULL) {
        return; /*no nodes to delete*/
    }
    else{
        while (current_to_del->next != NULL) {
            current_to_del = current_to_del->next;
            freeBoard(current_to_del->prev->board);
            free(current_to_del->prev);
            }
        }
        freeBoard(current_to_del->board);
        free(current_to_del);
    }

void InsertAtTail(cell **board, list *lst) {
    node *temp = lst->current;
    node *new_node = CreateNode(board);
    if (temp == NULL) { /*first move*/
        lst->head->next = new_node;
        new_node->prev = lst->head;
        lst->current = new_node;
        lst->tail = new_node;
        return;
    }
    else if (temp != lst->tail){
        DeleteNextNodes(temp, lst);
    }
    else {
        lst->tail->next = new_node;
        new_node->prev = lst->tail;
        lst->tail = new_node;
        lst->current = new_node;
    }
}


void freeList(list * lst){
    node * current = lst->head;
    DeleteNextNodes(current, lst);
    freeBoard(lst->current->board);
    free(current);
    free(lst);
    lst = NULL;
}

void printBoardChanges(cell ** old_board, cell ** new_board) {
    int i, j, old_val, new_val;
    int N = rows * cols;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            old_val = old_board[i][j].number;
            new_val = new_board[i][j].number;
            if (old_val != new_val) {
                if (old_val == 0) {
                    printf("Redo %d,%d: from - to %d\n", j + 1, i + 1, new_val);
                } else if (new_val == 0) {
                    printf("Redo %d,%d: from %d to -\n", j + 1, i + 1, old_val);
                } else {
                    printf("Undo %d,%d: from %d to %d\n", j + 1, i + 1, old_val, new_val);
                }
            }
        }
    }
}

void Redo(list * lst, cell **board){
    node * new_current;
    if ((lst->current == lst->tail) || (lst->head->next == NULL)) {
        printf("Error: no moves to redo\n");
        return;
    }
    new_current = lst->current->next;
    copyBoard(new_current->board, board);
    printBoard(board);
    printBoardChanges(lst->current->board, new_current->board);
    lst->current = new_current;

}

void Undo(list * lst, cell **board){
    node * new_current;
    if ((lst->current == lst->head) || (lst->head->next == NULL)){
        printf("Error: no moves to undo\n");
        return;
    }
    new_current = lst->current->prev;
    copyBoard(new_current->board, board);
    printBoard(board);
    printBoardChanges(lst->current->board, new_current->board);
    lst->current = new_current;

}

void reset(list * lst, cell **board){
    DeleteNextNodes(lst->head, lst);
    copyBoard(lst->current->board, board);
    printBoard(board);
    printf("Board reset\n");

}


