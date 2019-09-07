

#include "stack.h"
#include <stdlib.h>
#include <stdio.h>




Iteration *createStackNode(int row , int column , int direction ){
    Iteration* newSolveItaration = (Iteration*)malloc(sizeof(Iteration));
    if(newSolveItaration == NULL)
        return ERROR_MEMORY_DIDNOT_ALLOCATED;
    newSolveItaration->row = row;
    newSolveItaration->column = column;
    newSolveItaration->direction = direction;
    newSolveItaration->next = NULL;
    return newSolveItaration;
}
stack* createStack(){
    stack* newStack = (stack*)malloc(sizeof(stack));
    if(newStack == NULL)
        return ERROR_MEMORY_DID_NOT_ALLOCATED;
    newStack->top = NULL;
    return newStack;
}


void Push(stack *stack, int row, int column, int direction) {
    Iteration* newSolveIteration = createStackNode(row, column, direction);
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
    else{
        stack->top = stack->top->next;
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


