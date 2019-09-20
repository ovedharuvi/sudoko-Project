
#ifndef SUDOKOPROJECT_GAME_H
#define SUDOKOPROJECT_GAME_H


#include "solver.h"
#include "doublyLinkedList.h"
#include <string.h>

#define DEFUALT_SIZE 3
#define UNUSED(x) (void)(x)

/* ----------GAME MODULE----------
 * This module has all the functions for the game itself.
 * such as commands, set commands info , loading and saving files , checking if the game is over and exiting accordingly.*/


/* -----Documentation of public Functions-----
 *      generate --
     *                  Return value: pointer to sudoko board if succeeded, NULL otherwise.
     *                  Param:  sudokoBoard* board - the starting board we want to generate from.
     *                          int numOfLegalValues - the function will randomly choose numOfLegalValues cells to
     *                                                 fill with random valid value.
     *                          int numOfFixedCells - if the board is solvable after filling random values.
     *                                                the function will randomly choose numOfFixedCells to keep.
 *      validate --
     *                  Return value: StatusType - TRUE if the board is solvable, FALSE otherwise.
     *                  Param : sudokoBoard* board - a board to check.
     *                  Other: If the board is SOLVABLE the function will update the solution value for all cells
     *                          according to the solution found.
 *      guess --
     *                  Return value: sudokoBoard* - a pointer to a board after using LP to guess values ,
     *                                              NULL if the LP problem isn't feasible.
     *                  Param: sudokoBoard* board - a board to guess solution to.
     *                         float threshold - this threshold will use as a lower bound for the guess,
     *                                              i.e after using the LP algorithm each value gets a score, and only values
     *                                              with a score greater than the threshold will be considered.
     *  guess hint --
     *                   Return value: void
     *                   Param: sudokoBoard* board - the game board.
     *                          int Row, Column - the desired cell to get a hint to.
     *                   Other: this function is using printf to console, prints the score of the specified cell
     *                          in the LP solution.
     *                          if the board is unsolvable the function will print error massage.
 *     numOfSolutions --
 *                       Return Value : int numOfSols - represent the number of various solutions that a given board has.
 *                       Param : sudokoBoard* board - the game board.
 *                       Other : this function is an exhaustive backtracking algorithm and may take sometime.
 *
 *
 *                  */
void init_game();  /*initialize the command array and sends welcome message*/
void exit_game(sudokoBoard *board_ptr, int is_exit_program);
StatusType check_game_over(MODE mode, sudokoBoard *board, CmdInfo cmdInfo);
extern int MarkErrors;
#endif
