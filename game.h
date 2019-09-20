
#ifndef SUDOKOPROJECT_GAME_H
#define SUDOKOPROJECT_GAME_H


#include "solver.h"
#include "doublyLinkedList.h"
#include <string.h>

#define DEFUALT_SIZE 3
#define UNUSED(x) (void)(x)

/* ----------GAME MODULE----------
 * This module has all the functions for the game itself.
 * such as commands, commands info and checking if the game is over and exiting accordingly.*/


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
StatusType solve_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum);/*loads the board x and changes the MODE to SOLVE_MODE*/
StatusType edit_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum);
/*change the MODE to EDIT_MODE. in case of path given loads a board (with mark errors) else creates empty 9*9 board (from board.h) */
StatusType mark_errors_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum);/*change mark errors field in the board*/
StatusType print_board_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum);
/*mark errors and MODE needs to be considered.in Edit MarkErrors is on*/
StatusType set_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum);
/*sets value in the board according to the MODE. if the cell is error updates the field in the cell.
 * if game is over acts according to the instruction f in set */
StatusType validate_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum);
/* if the board is erroneous returns error. calls validate from the solver and prints whether the board has solution according to the returned value from solver*/
StatusType guess_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum);
/* if the board is erroneous returns error. calls guess from the solver*/
StatusType generate_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum);
/*calls guess from the solver. prints error if needed*/
StatusType undo_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum);/*calls to listUndo() from doubly linked */
StatusType redo_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum);/*calls to listRedo() from doubly linked */
StatusType save_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum);/*saves the board to the file x given according to the MODE*/
StatusType hint_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum);
/*calls validate and returns the field solution_value of this cell or error.consider fixed cell,filled cell,erroneous.*/
StatusType guess_h_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum);
/* if the board is erroneous returns error. calls guessHint from the solver*/
StatusType num_s_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum);
/* if the board is erroneous returns error. calls numOfSolutions from the solver*/
StatusType autofill_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum);
/*fills with set command the cells that has single value in the field potentialValues in cell struct*/
StatusType reset_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum);
/*listUndo from doublyLinkedList to the header */
StatusType exit_program_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum);
/*releases memory by destroyBoard from solver, destroylist ? . prints exit message.*/


void SetCmdArray(); /*initialize Command Array*/
sudokoBoard * load(char *path);/*case when edit with no params load gets NULL and generates 9*9 board*/
StatusType is_game_over(sudokoBoard *board_ptr); /*returns TRUE if all board is filled else FALSE*/
void exit_game(sudokoBoard *board_ptr, int is_exit_program);

extern int MarkErrors;
#endif
