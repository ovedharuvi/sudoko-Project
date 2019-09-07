
#ifndef SUDOKOPROJECT_STACK_H
#define SUDOKOPROJECT_STACK_H
#include "board.h"

struct stackNode {
    int row;
    int column;
    struct stackNode* next;
} ;

typedef struct stackNode Iteration;
typedef struct{
    Iteration* top;

} stack;

stack*  createStack();
void Push(stack *stack, int row, int column);
Iteration *top(stack* stack);
void Pop(stack *stack);
void destroyStack(stack *stack);
#endif //SUDOKOPROJECT_STACK_H
