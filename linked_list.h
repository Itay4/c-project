/** @file linked_list.h
 *  @brief linked list header file.
 *
 *  This header contains the prototypes for the linked list module.
 *
 *  @author Itay Keren (itaykeren)
 *  @author Rotem Bar (rotembar)
 *
 */

#ifndef SUDOKU_LINKED_LIST_H
#define SUDOKU_LINKED_LIST_H


/* -- Includes -- */
#include "main_aux.h"


/* -- Structs -- */
/**
 * @brief Node structure of a linked list.
 * each node is use to represent a game move.
 * node contains board field to save board status after a move was made.
 * node contains next field to point on next node (move) on linked list (moves list) a potential redo move.
 * node contains prev field to point on prev node (move) on linked list (moves list) a potential undo move.
 * if there are no prev or next nodes fields point to NULL.
 * Detailed explanation.
 */
typedef struct node {
    cell** board;
    struct node *next;
    struct node *prev;
}node;


/**
 * @brief List structure of a linked list.
 * list is used to represent game moves list
 * list contains head field to point at node representing the beginning of a list (origin board of the game).
 * list contains current field to point on node representing the current move of a list (last move which was made including redos and undos).
 * list contains tail field to point on node representing the last move of a list (last move which was made not including redos and undos ).
 * if tail field != current field undos were made by the user. after new move will be made redos will be deleted.
 * if no moves were made tail points to NULL
 * Detailed explanation.
 */
typedef struct list {
    node* head;
    node* current;
    node* tail;
}list;


/** @brief Returns a new game moves list for a new sudoku game.
 *  @param starting board of the game.
 *  @return moves list.
 */
list* create_list(cell** board);


/** @brief Updates ame moves list with a new move to be added at the end of the list.
 *  if undos were made deletes available redos.
 *  @param board sudoku board status after a move was made.
 *  @param lst moves list to be updated.
 *  @return void.
 */
void insert_at_tail(cell** board, list* lst);


/** @brief Frees memory of given game moves list.
 *  @param lst moves list to be freed.
 *  @return void.
 */
void free_list(list* lst);

/** @brief Updates game moves list and given sudoku board by redoing last move.
 *  @parm lst moves list which contains move to be redo to be updated.
 *  @param board sudoku board to be updated after move was redone.
 *  @param mode game mode.
 *  @return void.
 */
void redo(list* lst, cell** board, char mode);


/** @brief Updates game moves list and given sudoku board by undoing last move.
 *  @parm lst moves list which contains move to be undo to be updated.
 *  @param board sudoku board to be updated after move was undone.
 *  @param mode game mode.
 *  @return void.
 */
void undo(list* lst, cell** board, char mode);


/** @brief Updates game moves list and given sudoku board by resetting board to origin board
 *  deleting all moves except for the origin board.
 *  @parm lst moves list which contains origin board at the haed of the list.
 *  @param board sudoku board to be updated after reseted to origin.
 *  @param mode game mode.
 *  @return void.
 */
void reset(list* lst, cell** board, char mode);


#endif /*SUDOKU_LINKED_LIST_H*/
