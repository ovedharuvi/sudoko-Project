//
// Created by oved on 02/08/2019.
//

#include "game.h"

void maintain_list(CmdType cmdType, sudokoBoard *board, sudokoBoard *copy, int oldValue, int newValue, int row, int column);

void compare_board(sudokoBoard *board, sudokoBoard *copy);


StatusType check_range(int row, int column, int value, int size);


void make_board_equal(sudokoBoard *board_ptr, sudokoBoard *copy, CmdType cmdType) {
    int i,j,n,oldValue,newValue;
    n = board_ptr->boardSize;
    for(i =0; i<n;i++){
        oldValue = board_ptr->board[i][j].value;
        newValue = copy->board[i][j].value;
        if (oldValue!= newValue){
            InsertAction(oldValue,newValue,i,j,0,cmdType);
        }
    }
    /////////////////////////////continue here
}

sudokoBoard * load(char *path) {
    return 0;
}

StatusType is_game_over(sudokoBoard *board_ptr) {
    int i,j,n;
    n = board_ptr->boardSize;
    for(i =0; i<n;i++){
        if (board_ptr->board[i][j].value == 0){
            return FALSE;
        }
    }
    return TRUE;
}

StatusType solve_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    board = load(paramsArray[paramNum-1]);/*first and only parameter of solve function - string of the path*/
    if (board==NULL)/*check if load is valid*/
        return FALSE;
    *p_mode = SOLVE_MODE;
    destroyList();/*maintain the doublylinkedlist*/

    return TRUE;
}

StatusType edit_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    if(paramsArray[0][0] == '\0'){///////check condition, if no parameter
        board = createBoard(DEFUALT_SIZE , DEFUALT_SIZE , FALSE);
    }
    else {
        board = load(paramsArray[paramNum - 1]);/*first and only parameter of solve function - string of the path*/
    }
    //change mark errors to true ?
    if (board==NULL)/*check if load is valid*/
        return FALSE;

    *p_mode = EDIT_MODE;
    destroyList();/*maintain the doublylinkedlist*/
    return FALSE;
}

StatusType mark_errors_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    int param;
    param = atoi(paramsArray[0]);
    if(param == 0 || param== 1 ) {
        board->markErrors = param;
        return TRUE;
    }
    return error_message(incorrect_range,Mark);
}

StatusType print_board_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    int mark_errors;
    mark_errors = board->markErrors;
    if (*p_mode == EDIT_MODE){
        mark_errors =1;
    }
    printBoard(0,board,mark_errors);
    return FALSE;
}

StatusType set_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    int i, j,value,n;
    StatusType status;
    i = atoi(paramsArray[0])-1;/* -1 beacause the user enters in 1 base */
    j = atoi(paramsArray[1])-1;
    value = atoi(paramsArray[2]);
    n = board->boardSize;

    /*check range of params is valid*/
    status = check_range(i, j, value, n);
    if (status == FALSE) {
        return error_message(incorrect_range,Set);
    }

    /*check if fixed and solve mode*/
    if (*p_mode ==SOLVE_MODE && board->board[i][j].is_fixed ==1){
        return error_message(board_invalid_for_cmd,Set);
    }

    ////////// I don't maintain errorneus because if a user sets an errorneus cell we need to check everything

    /*maintain doubly linked list*/
    InsertAction(board->board[i][j].value,value,i,j,0,SET);

    /*Set action itself*/
    board->board[i][j].value = value;

    status = is_game_over(board);

    if (status == TRUE ) {
        if (is_erroneous(board) == TRUE){
            //return error of errorneus
        }
        else game_over();//release everything and back to INIT

    }

    return status;
}

StatusType validate_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    int result;
    result = validate(board);
    printf(result != 1 ? "The board is unsolvable." : "The board is solvable.");
    return FALSE;
}


StatusType guess_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    sudokoBoard * copy;
    //////////////////////// who sends this error ??
    if (is_erroneous(board) == TRUE)
        return error_message(board_invalid_for_cmd,Validate);
    copy = guess(board,0);////what float am I suppose to give ?, board and not int ?
    make_board_equal(board,copy,GUESS);




    return FALSE;
}

StatusType generate_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    int x,y;
    x = atoi(paramsArray[0]);
    y = atoi(paramsArray[1]);

    return FALSE;
}


StatusType check_range(int row, int column, int value, int size) {
    if (row < 0 || row > size || column < 0 || value < 1 || value > size+1)//////////CHECK IF NEEDS TO HAVE LOWER BOUND
    return FALSE;

    return TRUE;
}

