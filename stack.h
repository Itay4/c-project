/** @file stack.h
 *  @brief stack header file.
 *
 *  This header contains the prototypes for the stack module.
 *
 *  @author Itay Keren (itaykeren)
 *  @author Rotem Bar (rotembar)
 *
 */

#ifndef STACK_H
#define STACK_H

#include "main_aux.h"

/**
 * @brief element structure representing an element in a stack.
 * element contains data field which contains an array of integers.
 * element contains board field which contains a sudoku board.
 * element contains next field which points to the next element in the stack.
 */
typedef struct element {
    int* data;
    cell** board;
    struct element *next;
}element;


/**
 * @brief element structure representing a stack.
 * stack contains counter field which indicates the number of elements in a stack.
 * stack contains top field which points to the top element in the stack.
 */
typedef struct stack_t {
    int counter;
    element *top;
}stack;

/*
 *  @brief Initializes a stack
 *  @param stck - Stack to init
 */
void stack_initialize(stack* stck);

/*
 *  @brief Pushes element to stack
 *  @param data - Data to push in the element
 *  @param board - Board to push in the element
 *  @param stck - Stack to push element to
 */
void push(int* data, cell** board, stack* stck);


/*
 *  @brief Pops an element from the stack
 *  @param stck - Stack to pop element from
  * @return The popped element
 */
element* pop(stack* stck);

#endif /*STACK_H*/
