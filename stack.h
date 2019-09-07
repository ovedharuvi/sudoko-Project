
#ifndef SUDOKOPROJECT_STACK_H
#define SUDOKOPROJECT_STACK_H
#include "board.h"

struct stackNode {
    int row;
    int column;
    int direction;
    struct stackNode* next;
} ;

typedef struct stackNode Iteration;
typedef struct{
    Iteration* top;

} stack;

stack*  createStack();
void Push(stack *stack, int row, int column, int direction);
Iteration *top(stack* stack);
void Pop(stack *stack);
#endif //SUDOKOPROJECT_STACK_H
