//
// Created by oved on 02/08/2019.
//

#include "game.h"

ACTION* maintain_list(CmdType cmdType, char ** paramsArray, sudokoBoard *board,sudokoBoard *copy, ACTION* action);

void compare_board(sudokoBoard *board, sudokoBoard *copy);

ACTION* maintain_list(CmdType cmdType, char **paramsArray, sudokoBoard *board, sudokoBoard *copy, ACTION* action) {
    switch (cmdType){
        case (EDIT): destroyList(action); break;///////Oved
        case (SOLVE):
        case (SET): InsertAction(paramsArray,0,cmdType);break;
        case (AUTOFILL || GENERATE || GUESS): compare_board(board,copy);break;


    }
    return FALSE;
}

void compare_board(sudokoBoard *board, sudokoBoard *copy) {

}

sudokoBoard * load(char *path) {
    return 0;
}

int is_game_over(sudokoBoard *board) {
    return NO;
}

StatusType solve_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    board = load(paramsArray[paramNum-1]);/*first and only parameter of solve function - string of the path*/
    *p_mode = SOLVE_MODE;

    return FALSE;
}

StatusType edit_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    if(paramsArray[0][0] == '\0'){///////check condition, if no parameter
        board = createBoard(DEFUALT_SIZE , DEFUALT_SIZE , FALSE);
    }
    else {
        board = load(paramsArray[paramNum - 1]);/*first and only parameter of solve function - string of the path*/
    }
    //change mark errors to true ?

    *p_mode = EDIT_MODE;
    return FALSE;
}

StatusType mark_errors_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    if(paramsArray[0] != )////////////////////continue here
    return FALSE;
}

