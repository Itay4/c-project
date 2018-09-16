/** @file linked_list.c
 *  @brief linked list source file.
 *
 *  This module encapsulates the sudoku puzzle game moves list represented by a linked list.
 *  Includes functions to match all game commands related to moves list to be entered by the user (undo, redo, reset).
 *  Includes auxiliary functions that are relevant specifically to linked_list structure.
 *
 *  @author Itay Keren (itaykeren)
 *  @author Rotem Bar (rotembar)
 *
 */

/* -- Includes -- */
#include "linked_list.h"
#include "main_aux.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Allocates memory and creates returns a node to be later on added to game moves list represented by a linked list.
 * node board field is updated with given sudoku board (board after a changing command was executed).
 * prev and next field are updated to null and will be updated in the future when added to a list */
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
/* Allocates memory and creates and returns a linked list which intend to represent game moves list
 * first node in the list (head of list) is created based on given sudoku board (first board in current game)
 * fields head and current are updated to point on head of list,
 * tail is updated to null to mark a newly created list which only contain head*/
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

/* Frees given moves list by freeing memory of all nodes and list itself */
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

/* Updates the given game moves list by deleting all nodes beyond the current pointer
 * update current and tail field to point to newTail and adding it to end of the moves list */
void delete_next_nodes(node* newTail, list* lst) {
    node* currentToDel;
    if (newTail == NULL){
        return;
    }
    currentToDel = lst->current->next;
    /* adding newTail to the end of the list */
    newTail->next = NULL;
    newTail->prev = lst->current;
    lst->tail = newTail;
    lst->current->next = newTail;
    lst->current = newTail;
    if (currentToDel == NULL) {
        return; /* no nodes to delete */
    } else {
        while (currentToDel->next != NULL) {
            currentToDel = currentToDel->next;
            free_board(currentToDel->prev->board); /* deleting nodes by  memory freeing */
            free(currentToDel->prev);
        }
    }
    free_board(currentToDel->board);
    free(currentToDel);
}

/* Updates given game moves list by generating a new node based on given board and adding it to the end of the list
 * if undo was made and then a new move was made, redo moves are deleted. */
void insert_at_tail(cell** board, list* lst) {
    node* temp = lst->current;
    node* newNode = create_node(board);
    if (temp == lst->head) { /* first move of the game*/
        lst->head->next = newNode;
        newNode->prev = lst->head;
        lst->current = newNode;
        lst->tail = newNode;
        return;
    } else if (temp != lst->tail){ /* undo was made and then a new move was made */
        delete_next_nodes(newNode, lst); /* deleting available redos moves */
    } else { /* standard move */
        lst->tail->next = newNode;
        newNode->prev = lst->tail;
        lst->tail = newNode;
        lst->current = newNode;
    }
}

 /* Prints messages corresponding moves made from oldBoard to create newBoard
  * cmdType representing redo or undo command*/
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

/* updates given sudoku board and moves list by redoing next move available in moves list.
 * if no move available to redo (current has no next) print appropriate messages.
 * if a move was redo prints new board and changes made. */
void redo(list* lst, cell** board, char mode){
    node* newCurrent;
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

/* updates given sudoku board and moves list by undoing the last move available in moves list.
 * if no move available to undo (current has no prev) print appropriate messages.
 * if a move was undo prints new board and changes made. */
void undo(list* lst, cell** board, char mode){
    node* newCurrent;
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

/* updates given sudoku board and moves list by resetting board to originally loaded or generated board (head of the list).
 * all nodes of moves list except for the head are deleted.
 * prints the original board. */
void reset(list* lst, cell** board, char mode){
    if (lst->head->next == NULL){ /* no moves were made */
        print_board(board, mode);
        printf("Board reset\n");
        return;
    }
    delete_next_nodes(lst->head, lst);
    copy_board(lst->current->board, board);
    print_board(board, mode);
    printf("Board reset\n");
}