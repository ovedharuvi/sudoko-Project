#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#ifndef SUDOKOPROJECT_DOUBLYLINKEDLIST_H
#define SUDOKOPROJECT_DOUBLYLINKEDLIST_H
typedef enum {
    SOLVE = 0,EDIT = 1,MARK = 2, PRINT = 3, SET = 4, VALIDATE = 5, GUESS = 6, GENERATE = 7, UNDO = 8,
    REDO = 9, SAVE = 10, HINT = 11, GUESS_H = 12, NUM_S = 13, AUTOFILL = 14, RESET = 15, EXIT_GAME = 16, INVALID =17
}CmdType;

typedef struct{
    int oldValue;
    int newValue;
    int row;
    int column;
    int insertedByComputer;
    CmdType command;
}ACTION;

/*Insert Action after current action, delete all actions we've undo and free memory */
void InsertAction (int oldValue , int newValue, int row , int column , int insertedByComputer , CmdType command);

/*moves current action pointer to previous action , return data of current action, prints error if no action to undo */
ACTION* listUndo();

/*moves current action pointer to next action , return data of current action , prints error if no action to redo*/
ACTION* listRedo();

/*prints the data from the last action to the first action*/
void printLastToFirst();

/*print the data from the first action made to current action (action history)*/
void printFirstToCurrentAction();

void destroyList();
#endif //SUDOKOPROJECT_DOUBLYLINKEDLIST_H
