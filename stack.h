
#include "main_aux.h"

#ifndef STACK_H
#define STACK_H


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

void stack_initialize(stack* stck);

void push(int* data, cell** board, stack* stck);

element* pop(stack* stck);

#endif /*STACK_H*/
