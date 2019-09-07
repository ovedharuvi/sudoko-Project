//
// Created by oved on 02/08/2019.
//

#include "game.h"

void
maintain_list(CmdType cmdType, sudokoBoard *board, sudokoBoard *copy, int oldValue, int newValue, int row, int column);

void compare_board(sudokoBoard *board, sudokoBoard *copy);

void maintain_list(CmdType cmdType, sudokoBoard *board, sudokoBoard *copy, int oldValue, int newValue, int row,
                   int column) {
    switch (cmdType){
        case (EDIT):  destroyList(); break;///////Oved
        case (SOLVE): destroyList(); break;
        case (SET): InsertAction(oldValue,newValue,row,column,0,cmdType);break;
        case (AUTOFILL): make_equal_board(board,copy);break;
        case(GENERATE): make_equal_board(board,copy);break;
        case (GUESS):make_equal_board(board,copy);break;
        default:break;

    }
    return TRUE;
}

void make_equal_board(sudokoBoard *board, sudokoBoard *copy) {
    int n,m;
    n = board->boardSize;
    /////////////////////////////continue here
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

