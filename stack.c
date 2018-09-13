#include "stack.h"
#include "main_aux.h"
#include <stdio.h>
#include <stdlib.h>

void stack_initialize(stack* stck ) {
    /* Initializes a stack */
    stck->counter = 0;
    stck->top = NULL;
}


void push(int* data, cell** board, stack* stck ) {
    /* Pushes an element to top of stack*/
    element *e = NULL;
    e = (element*) (malloc(sizeof(element)));
    e->data = data;
    e->board = board;
    e->next = stck->top;
    stck->top = e;
    stck->counter++;
}

element* pop(stack* stck) {
    /* Gets the stack top elemnent data */
    element *e = NULL;
    e = stck->top;
    stck->top = stck->top->next;
    stck->counter--;
    return e;
}