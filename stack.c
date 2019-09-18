

#include "stack.h"
#include <stdlib.h>
#include <stdio.h>




Iteration *createStackNode(int row, int column) {
    Iteration* newSolveItaration = (Iteration*)malloc(sizeof(Iteration));
    if(newSolveItaration == NULL)
        return NULL;
    newSolveItaration->row = row;
    newSolveItaration->column = column;
    newSolveItaration->next = NULL;
    return newSolveItaration;
}
stack* createStack(){
    stack* newStack = (stack*)malloc(sizeof(stack));
    if(newStack == NULL)
        return NULL;
    newStack->top = NULL;
    return newStack;
}


void Push(stack *stack, int row, int column) {
    Iteration* newSolveIteration = createStackNode(row, column);
    if(newSolveIteration == NULL){
        return;
    }
    if(stack->top == NULL){
        stack->top = newSolveIteration;
        return;
    }
    else{
        newSolveIteration->next = stack->top;
        stack->top = newSolveIteration;
    }
}

Iteration* top(stack* stack){
    Iteration* temp = stack->top;
    if(stack->top == NULL){
        printf("Error : stack is empty.\n");
        return NULL;
    }
    return temp;
}

void Pop(stack *stack) {
    Iteration* temp = stack->top;
    if(stack->top == NULL){
        printf("Error : stack is empty , can't Pop.\n");
    }
    else{
        stack->top = stack->top->next;
        free(temp);
    }
}

void destroyStack(stack *stack){
    while(top(stack) != NULL){
        Pop(stack);
    }
    free(stack);
}

