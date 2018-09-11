
#ifndef SUDOKU90_LINKED_LIST_H
#define SUDOKU90_LINKED_LIST_H


#include "main_aux.h"

typedef struct node {
    cell ** board;
    struct node *next;
    struct node *prev;
}node;

typedef struct list
{
    node *head;
    node *current;
    node *tail;
}list;

node* CreateNode(cell **board);
list* CreateList(cell **board);
void InsertAtTail(cell **board, list *lst);
void DeleteNextNodes(node* new_tail, list *lst);
void freeList(list * lst);
void printBoardChanges(cell ** old_board, cell ** new_board);
void Redo(list * lst, cell **board);
void Undo(list * lst, cell **board);
void reset(list * lst, cell **board);


#endif /*SUDOKU90_LINKED_LIST_H*/
