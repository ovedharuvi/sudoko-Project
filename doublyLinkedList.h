
#include <stdio.h>
#include <stdlib.h>
#include "cmdType.c"
#ifndef SUDOKOPROJECT_DOUBLYLINKEDLIST_H
#define SUDOKOPROJECT_DOUBLYLINKEDLIST_H

/* ----------- DOUBLY LINKED LIST MODULE --------------
 *  This module is an implementation of doubly linked list including a pointer that represent the current action.*/
typedef struct{
    int oldValue;
    int newValue;
    int row;
    int column;
    int insertedByComputer;
    CmdType command;
}ACTION;




/*
 * Insert Action -
 *                  Return Value - void
 *                  Param        - oldValue , newValue , row , column , insertedByComputer , command
 *                  Other        - inserting action will cancel the option to redo previous actions.
 * listUndo      -
 *                  Return Value -  ACTION* a pointer to the previous action made by the user.
 *                                  NULL if there is no action to undo.
 *                  Param        -  none
 *                  Other        -
 * listRedo      -
 *                  Return Value - ACTION* a pointer to the last action the the user undo.
 *                                 NULL if the is no action to redo.
 *                  Param        -  None
 *                  Other        -
 * destroyList   -
 *                  Return Value -  void
 *                  Param        - none
 *                  Other        -
 *
 *
 *                  */
/*Insert Action after current action, delete all actions we've undo and free memory */
void InsertAction (int oldValue , int newValue, int row , int column , int insertedByComputer , CmdType command);

/*moves current action pointer to previous action , return data of current action, prints error if no action to undo */
ACTION* listUndo();

/*moves current action pointer to next action , return data of current action , prints error if no action to redo*/
ACTION* listRedo();

void destroyList();
#endif
