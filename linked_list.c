
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"
#include "main_aux.h"
#include "game.h"
#include "parser.h"

node* CreateNode(data x) {
    node* new_node = malloc(sizeof(node));
    new_node->d = x;
    new_node->prev = NULL;
    new_node->next = NULL;
    return new_node;
}

list* CreateList(data x){
    list * list = malloc(sizeof(list));
    list->head = CreateNode(x);
    list->tail = NULL;
    list->current = NULL;
    return list;
}


void DeleteNextNodes(node* new_tail, list *lst) {
    node * current = new_tail->next;
    new_tail->next = NULL;
    lst->tail = new_tail;
    lst->current = new_tail;
    if (current == NULL) {
        return; /*no nodes to delete*/
    }
    else{
        while (current->next != NULL) {
            current = current->next;
            free(current->prev);
        }
        free(current);
    }
}

void InsertAtTail(data d, list *lst) {
    node *temp = lst->current;
    node *new_node = CreateNode(d);
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
        temp = lst->tail;
        lst->tail->next = new_node;
        new_node->prev = lst->tail;
        lst->tail = new_node;
        lst->current = new_node;

    }
}


void DeleteList(list * lst){
    node * current = lst->head;
    while (current->next != NULL) {
        current = current->next;
        free(current->prev);
    }
    free(current);
    free(lst);
    lst = NULL;
}

void Redo(list * lst, cell **board){
    node * new_current;
    int row_index, col_index, new_val, old_val;
    if ((lst->current == lst->tail) || (lst->head->next == NULL)) {
        printf("Error: no moves to redo\n");
        return;
    }
    new_current = lst->current->next;
    row_index = new_current->d.row_index;
    col_index = new_current->d.col_index;
    new_val = new_current->d.value;
    old_val = board[row_index - 1][col_index - 1].number;
    lst->current = new_current;
    copyBoard(new_current->d.board, board);
    printBoard(board);
    if (old_val == 0){
        printf("Redo %d,%d: from - to %d\n",col_index, row_index,new_val);
    }
    else if (new_val == 0) {
        printf("Redo %d,%d: from %d to -\n", col_index, row_index, old_val);
    }
    else{
        printf("Redo %d,%d: from %d to %d\n",col_index, row_index, old_val, new_val);
    }

}

void Undo(list * lst, cell **board){
    node * new_current;
    int row_index, col_index, old_val, new_val;
    if ((lst->current == lst->head) || (lst->head->next == NULL)){
        printf("Error: no moves to undo\n");
        return;
    }
    row_index = lst->current->d.row_index;
    col_index = lst->current->d.col_index;
    old_val = lst->current->d.value;
    new_current = lst->current->prev;
    lst->current = new_current;
    new_val = lst->current->d.board[row_index - 1][col_index - 1].number;
    copyBoard(lst->current->d.board, board);
    printBoard(board);
    if (old_val == 0){
        printf("Redo %d,%d: from - to %d\n",col_index, row_index,new_val);
    }
    else if (new_val == 0) {
        printf("Redo %d,%d: from %d to -\n", col_index, row_index, old_val);
    }
    else {
        printf("Undo %d,%d: from %d to %d\n", col_index, row_index, old_val, new_val);
    }

}
