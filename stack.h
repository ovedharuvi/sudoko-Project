
#ifndef SUDOKOPROJECT_STACK_H
#define SUDOKOPROJECT_STACK_H
#include "board.h"


/*---------- STACK MODULE -----------
 * Linked list based stack.
 * Each stack node contains row and column as data.*/
struct stackNode {
    int row;
    int column;
    struct stackNode* next;
} ;

typedef struct stackNode Iteration;
typedef struct{
    Iteration* top;

} stack;

/*Documentation for public functions
 * createStack --
 *                      Return Value - stack* -- a point the a new stack.
 *                      Param: None
 *                      Other : setting top to NULL, returns NULL if memory didn't allocated successfully.
 * Push --
 *                      Return Value - void
 *                      Param - stack* - stack to push to.
 *                              int row,column - the row and column for the stackNode created.
 * top --
 *                      Return Value - Iteration* - a pointer to the iteration (current cell) at the top of the stack (last in)
 *                      Param - stack* - A stack to access it's top.
 * Pop --
 *                      Return Value - void
 *                      Param - stack* the stack to Pop it's top
 * destroyStack -
 *                      Return Value - void
 *                      Param - stack* - A stack to destroy.
 *                      Other - use pop until stack is empty.*/

stack*  createStack();
void Push(stack *stack, int row, int column);
Iteration *top(stack* stack);
void Pop(stack *stack);
void destroyStack(stack *stack);
#endif //SUDOKOPROJECT_STACK_H
