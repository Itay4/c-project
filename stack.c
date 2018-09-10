

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "stack.h"

StackNode* newNode(SnapShotStruct snapshot)
{
    StackNode* stackNode = (StackNode*) malloc(sizeof(StackNode));
    stackNode->snapshot.i = snapshot.i;
    stackNode->snapshot.j = snapshot.j;
    stackNode->snapshot.counter = snapshot.counter;
    stackNode->snapshot.stage = snapshot.stage;
    stackNode->next = NULL;
    return stackNode;
}

int empty(StackNode *root)
{
    return !root;
}

void push(StackNode** root, SnapShotStruct snapshot)
{
    StackNode* stackNode = newNode(snapshot);
    stackNode->next = *root;
    *root = stackNode;
}

SnapShotStruct pop(StackNode** root)
{
    SnapShotStruct popped;
    StackNode* temp = *root;
    if (empty(*root))
        return popped;
    *root = (*root)->next;
    popped = temp->snapshot;
    free(temp);

    return popped;
}

SnapShotStruct top(StackNode* root)
{
    SnapShotStruct snapshot;
    if (empty(root))
        return snapshot;
    snapshot = root->snapshot;
    return snapshot;
}
