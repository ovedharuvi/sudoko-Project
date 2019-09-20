
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


/*------------General Information----------
 * All orders function are build in the same way : starts with 'cmd'.
 * The return value is StatusType: if the function failed - returns FALSE and prints error message.
 * if the function */

/* -----Documentation of public Functions-----
 *
 *
 *                  */
void init_game();  /*sends welcome message*/
void exit_game(sudokoBoard *board_ptr, int is_exit_program);
void SetCmdArray(); /*initialize Command Array*/
void maintain_erroneous(sudokoBoard * board);/*maintains erroneous field of each cell in the board*/
StatusType check_game_over(sudokoBoard *board, CmdInfo cmdInfo);
extern int MarkErrors;
#endif
