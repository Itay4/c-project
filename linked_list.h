//
// Created by Rotem on 08/09/2018.
//

#ifndef NEW_SUDUKO_LINKED_LIST_H
#define NEW_SUDUKO_LINKED_LIST_H

#include "main_aux.h"

typedef struct data {
    int col_index;
    int row_index;
    int value;
    cell **board;
}data;

typedef struct node {
    data d;
    struct node *next;
    struct node *prev;
}node;

typedef struct list
{
    node *head;
    node *current;
    node *tail;
}list;

node* CreateNode(data x);
list* CreateList(data x);
void InsertAtTail(data d, list *lst);
void DeleteNextNodes(node* new_tail, list *lst);
void DeleteList(list * lst);
void Redo(list * lst, cell **board, size_t *rows, size_t *cols, int markErrors);
void Undo(list * lst, cell **board, size_t *rows, size_t *cols, int markErrors);
#endif /*NEW_SUDUKO_LINKED_LIST_H*/

