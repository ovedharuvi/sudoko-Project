
#include <stdio.h>
#include <stdlib.h>
#include "cmdType.h"
#ifndef SUDOKOPROJECT_DOUBLYLINKEDLIST_H
#define SUDOKOPROJECT_DOUBLYLINKEDLIST_H


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
/*void printLastToFirst();*/

/*print the data from the first action made to current action (action history)*/

void destroyList();
#endif
