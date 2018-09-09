

#ifndef SUDOKU90_STACK_H
#define SUDOKU90_STACK_H


typedef struct SnapShotStruct {
    int i;
    int j;
    int counter;
    int stage;
}SnapShotStruct;

typedef struct StackNode
{
    struct SnapShotStruct snapshot;
    struct StackNode* next;
}StackNode;

struct StackNode* newNode(SnapShotStruct snapshot);

int empty(StackNode *root);

void push(StackNode** root, SnapShotStruct snapshot);

SnapShotStruct pop(StackNode** root);

SnapShotStruct top(StackNode* root);

#endif /*SUDOKU90_STACK_H*/
