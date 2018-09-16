/** @file stack.h
 *  @brief stack header file.
 *
 *  This header contains the prototypes for the stack module.
 *
 *  @author Itay Keren (itaykeren)
 *  @author Rotem Bar (rotembar)
 *
 */

#ifndef SUDOKU_STACK_H
#define SUDOKU_STACK_H


/* -- Includes -- */
#include "main_aux.h"





/* -- Structs -- */


typedef struct element {
    int* data;
    cell** board;
    struct element *next;
}element;

typedef struct stack_t
{
    int counter;
    element *top;
}stack;


/**
 *  @brief Initializes a stack.
 *  @param stck - Stack to init.
 *  @return void.
 */
void stack_initialize(stack* stck);


/**
 *  @brief Pushes element to stack
 *  @param data - Data to push in the element
 *  @param board - Board to push in the element
 *  @param stck - Stack to push element to
 *  @return void.
 */
void push(int* data, cell** board, stack* stck);


/**
 *  @brief Pops an element from the stack
 *  @param stck - Stack to pop element from
  * @return The popped element
 */
element* pop(stack* stck);

#endif /*SUDOKU90_STACK_H*/
