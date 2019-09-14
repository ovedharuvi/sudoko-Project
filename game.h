//
// Created by oved on 02/08/2019.
//

#ifndef SUDOKOPROJECT_GAME_H
#define SUDOKOPROJECT_GAME_H


#include "solver.h"
#include "doublyLinkedList.h"

#endif //SUDOKOPROJECT_GAME_H

#define DEFUALT_SIZE 3


StatusType solve_cmd(char ** paramsArray,sudokoBoard *board,MODE *p_mode,int paramNum);/*loads the board x and changes the MODE to SOLVE_MODE*/
StatusType edit_cmd(char ** paramsArray,sudokoBoard *board,MODE *p_mode,int paramNum);
/*change the MODE to EDIT_MODE. in case of path given loads a board (with mark errors) else creates empty 9*9 board (from board.h) */
StatusType mark_errors_cmd(char ** paramsArray,sudokoBoard *board,MODE *p_mode,int paramNum);/*change mark errors field in the board*/
StatusType print_board_cmd(char ** paramsArray,sudokoBoard *board,MODE *p_mode,int paramNum);
/*mark errors and MODE needs to be considered.in Edit mark_errors is on*/
StatusType set_cmd(char ** paramsArray,sudokoBoard *board,MODE *p_mode,int paramNum);
/*sets value in the board according to the MODE. if the cell is error updates the field in the cell.
 * if game is over acts according to the instruction f in set */
StatusType validate_cmd(char ** paramsArray,sudokoBoard *board,MODE *p_mode,int paramNum);
/* if the board is erroneous returns error. calls validate from the solver and prints whether the board has solution according to the returned value from solver*/
StatusType guess_cmd(char ** paramsArray,sudokoBoard *board,MODE *p_mode,int paramNum);
/* if the board is erroneous returns error. calls guess from the solver*/
StatusType generate_cmd(char ** paramsArray,sudokoBoard *board,MODE *p_mode,int paramNum);
/*calls guess from the solver. prints error if needed*/
StatusType undo_cmd(char ** paramsArray,sudokoBoard *board,MODE *p_mode,int paramNum);/*calls to listUndo() from doubly linked */
StatusType redo_cmd(char ** paramsArray,sudokoBoard *board,MODE *p_mode,int paramNum);/*calls to listRedo() from doubly linked */
StatusType save_cmd(char ** paramsArray,sudokoBoard *board,MODE *p_mode,int paramNum);/*saves the board to the file x given according to the MODE*/
StatusType hint_cmd(char ** paramsArray,sudokoBoard *board,MODE *p_mode,int paramNum);
/*calls validate and returns the field solution_value of this cell or error.consider fixed cell,filled cell,erroneous.*/
StatusType guess_h_cmd(char ** paramsArray,sudokoBoard *board,MODE *p_mode,int paramNum);
/* if the board is erroneous returns error. calls guessHint from the solver*/
StatusType num_s_cmd(char ** paramsArray,sudokoBoard *board,MODE *p_mode,int paramNum);
/* if the board is erroneous returns error. calls numOfSolutions from the solver*/
StatusType autofill_cmd(char ** paramsArray,sudokoBoard *board,MODE *p_mode,int paramNum);
/*fills with set command the cells that has single value in the field potentialValues in cell struct*/
StatusType reset_cmd(char ** paramsArray,sudokoBoard *board,MODE *p_mode,int paramNum);
/*listUndo from doublyLinkedList to the header */
StatusType exit_program_cmd(char ** paramsArray,sudokoBoard *board,MODE *p_mode,int paramNum);
/*releases memory by destroyBoard from solver, destroylist ? . prints exit message.*/


static CmdInfo  CmdArray[MAX_CMD] ; /*={Solve,Edit,Mark,Print,Set,Validate,Guess,Generate,Undo,Redo,Save,Hint,Guess_h,Num_s,Autofill,Reset_Game,Exit};*/
void SetCmdArray();
sudokoBoard * load(char *path);/*case when edit with no params load gets NULL and generates 9*9 board*/
StatusType is_game_over(sudokoBoard *board_ptr);
void exit_program();
StatusType is_erroneous(sudokoBoard * board); /*returns TRUE if a board is erroneous else FALSE*/
