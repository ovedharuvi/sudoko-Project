#ifndef SUDOKOPROJECT_BOARD_H
#define SUDOKOPROJECT_BOARD_H


/*  ---------- BOARD MODULE ----------
 *
 * This module is a data structure of the gaming board.
 * Contains the data structure itself and basic functions for boards
 * like create board, copy board , destroy board and check all kind of things in a given board*/
#include "status.h"
typedef struct {
    int is_fixed; /*0 if fixed 1 if not fixed */
    int is_erroneus; /*0 if erroneus 1 if not*/
    int value; /*0 if empty*/
    int solution_value; /*0 if empty*/

} cell;
typedef struct {
    cell **board;
    int heightOfBlock;
    int widthOfBlock;
    int boardSize;
} sudokoBoard;

/* Public functions documentation
 * CreateBoard -
 *                  Return Value -  sudokoBoard* - a pointer to sudoko board, all fields and values are set to 0.
 *                                                  NULL - if memory error occurred.
 *                  Param - height of block , width of block - determine the dimensions of the board.
 *                  Other - IMPORTANT - after using this function need to check if memory allocated successfully.
 * CopyBoard -
 *                  Return Value -  sudokoBoard* - a pointer to sudoko board, which all fields and values are the same
 *                                                  to the given board.
 *                  Param -         sudokoBoard* board - a board to make copy of.
 *                  Other - IMPORTANT - after using this function need to check if memory allocated successfully.
 * DestroyBoard -
 *                  Return Value - void
 *                  Param - board* - A board to destroy.
 *                  Other -
 * PrintBoard -
 *                  Return Value -  void
*                   Param -         int what To Print -- 0 prints cell value board , 1 prints solution value board (for debug)
*                                   int mark errors   -- print '*' after erroneous value.
*                                   int mark fixed    -- print '.' after fixed value.
 *                  Other -
 * checkIfValid -
 *                  Return Value - TRUE if v is valid for cell [row][column] , FALSE otherwise.
 *                  Param        - sudokoBoard* board.
 *                                 int value - value to check if valid.
 *                                 int row , column - cell index in board.
 *                                 int to mark errors - if to mark errors =1 -->
 *                                                          if value isn't valid the cell (or cells) that made him invalid will be
 *                                                          marked as erroneous.
 *                                                      otherwise -->
 *                                                          erroneous values are not allowed.
 * is erroneous -   Return Value - TRUE if an erroneous cell exist.
 *                                 FALSE otherwise.
 *                  Param        - board.
 *                  Other        -
 *      */
sudokoBoard *createBoard(int heightOfBlock, int widthOfBlock);
sudokoBoard *copyBoard(sudokoBoard *board);
void destroyBoard(sudokoBoard *board);
void printBoard(int whatToPrint, sudokoBoard *sudokoBoard, int mark_errors, int mark_fixed);            /*what to print = 0 --> print cell value
 *                                                                        what to print = 1 --> print cell solution value*/

StatusType checkIfValid(sudokoBoard *board, int value, int row, int column, int to_mark_error);
StatusType is_erroneous(sudokoBoard * board);
#endif
