/** @file stack.c
 *  @brief stack source file.
 *
 *  This module implements stack data structure.
 *  Includes basic functions required to maintain a stack.
 *
 *  @author Itay Keren (itaykeren)
 *  @author Rotem Bar (rotembar)
 *
 */

/* -- Includes -- */
#include "stack.h"
#include "main_aux.h"
#include <stdio.h>
#include <stdlib.h>

/* Initializes a stack */
void stack_initialize(stack* stck ) {
    stck->counter = 0;
    stck->top = NULL;
}

/* Pushes an element to the top of stack by changing pointers and increasing counter*/
void push(int* data, cell** board, stack* stck) {
    element *e = NULL;
    e = (element*) (malloc(sizeof(element)));
    if (e == NULL) {
        memory_error("Memory allocation failed\n");
        exit(0);
    }
    e->data = data;
    e->board = board;
    e->next = stck->top;
    stck->top = e;
    stck->counter++;
}

/* Gets the stack top elemnent data by fetching it and decreasing the counter */
element* pop(stack* stck) {
    element *e = NULL;
    e = stck->top;
    stck->top = stck->top->next;
    stck->counter--;
    return e;
}