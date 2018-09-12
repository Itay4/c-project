
#ifndef SUDOKU90_LINKED_LIST_H
#define SUDOKU90_LINKED_LIST_H


#include "main_aux.h"

typedef struct node {
    cell ** board;
    struct node *next;
    struct node *prev;
}node;

typedef struct list {
    node *head;
    node *current;
    node *tail;
}list;

node* create_node(cell **board);
list* create_list(cell **board);
void insert_at_tail(cell **board, list *lst);
void delete_next_nodes(node* new_tail, list *lst);
void free_list(list * lst);
void print_board_changes(cell ** oldBoard, cell ** newBoard, char* cmdType);
void redo(list * lst, cell **board, char mode);
void undo(list * lst, cell **board, char mode);
void reset(list * lst, cell **board, char mode);


#endif /*SUDOKU90_LINKED_LIST_H*/
