//
// Created by oved on 02/08/2019.
//

#include "game.h"


StatusType check_range(int row, int column, int value, int size);


void do_set_by_action(ACTION action, sudokoBoard *board, int is_undo);

void maintain_errorneues(int row, int column, int value, sudokoBoard *board);

void make_board_equal(sudokoBoard *board_ptr, sudokoBoard *copy, CmdType cmdType) {
    int i,j,n,oldValue,newValue;
    n = board_ptr->boardSize;
    for(i =0; i<n;i++){
        oldValue = board_ptr->board[i][j].value;
        newValue = copy->board[i][j].value;
        if (oldValue!= newValue){
            board_ptr->board[i][j].value = newValue;
            InsertAction(oldValue,newValue,i,j,FALSE,cmdType);
        }
    }

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
        mark_errors = TRUE;
    }
    printBoard(FALSE,board,mark_errors);
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
        return error_message(fixed_cell,Set);
    }

    maintain_errorneues(i,j,value,board);

    /*maintain doubly linked list*/
    InsertAction(board->board[i][j].value,value,i,j,0,SET);

    /*Set action itself*/
    board->board[i][j].value = value;

    status = is_game_over(board);

    if (status == TRUE ) {
        if (is_erroneous(board) == TRUE){
            return error_message(board_errorneus,Set);/*return error of errorneus*/
        }
        else game_over();//release everything and back to INIT

    }

    return status;
}

void maintain_errorneues(int row, int column, int value, sudokoBoard *board) {


}

StatusType validate_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    int result;
    result = validate(board);
    printf(result != SOLVABLE ? "The board is unsolvable." : "The board is solvable.");
    return FALSE;
}


StatusType guess_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    sudokoBoard * copy;
    //////////////////////// who sends this error ??
    if (is_erroneous(board) == TRUE)
        return error_message(board_errorneus,Validate);
    copy = guess(board,0);////what float am I suppose to give ?, board and not int ?
    make_board_equal(board,copy,GUESS);





    return FALSE;
}

StatusType generate_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    int x,y;
    ///////////////////////////////////
    x = atoi(paramsArray[0]);
    y = atoi(paramsArray[1]);
    generate(board,x,y);
    return FALSE;
}

StatusType undo_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    ACTION * action_ptr;
    CmdType cmd;
    action_ptr = listUndo();
    if (action_ptr == NULL)
        return error_message(no_moves_error,Undo);
    cmd = action_ptr->command;
    switch (cmd){
        case GENERATE:
        case AUTOFILL:
        case GUESS:
            while(action_ptr->insertedByComputer == TRUE){
                do_set_by_action(*action_ptr, board, TRUE);
                action_ptr = listUndo();}
             break;
        case SET:
            do_set_by_action(*action_ptr, board, TRUE);
            break;
        default:
            return error_message(invalid_move, CmdArray[cmd]);
    }


    return FALSE;
}

void do_set_by_action(ACTION action, sudokoBoard *board, int is_undo) {
    int value,i,j;
    i = action.row;
    j = action.column;
    if (is_undo == TRUE){
        value = action.oldValue;
    } else{value = action.newValue;}
    board->board[i][j].value = value;

}


StatusType redo_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    ACTION *action_ptr;
    CmdType cmd;
    action_ptr = listRedo();
    if (action_ptr == NULL)
        return error_message(no_moves_error, Redo);
    cmd = action_ptr->command;
    switch (cmd) {
        case GENERATE:
        case AUTOFILL:
        case GUESS:
            while (action_ptr->insertedByComputer == TRUE) {
                do_set_by_action(*action_ptr, board, FALSE);
                action_ptr = listRedo();
            }
            break;
        case SET:
            do_set_by_action(*action_ptr, board, FALSE);
            break;
        default:
            return error_message(invalid_move, CmdArray[cmd]);

    }
    return FALSE;
}

StatusType save_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    return FALSE;
}

StatusType hint_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    int x,y ;
    if (is_erroneous(board) == TRUE)
        return error_message(board_errorneus,Hint);
    x = atoi(paramsArray[0])-1;
    y = atoi(paramsArray[1])-1;
    if (board->board[x][y].is_fixed)
        return error_message(fixed_cell,Hint);
    if (board->board[x][y].value != 0)
        return error_message(non_empty_cell,Hint);
    if (validate(board)!= SOLVABLE)
        return error_message(unsolvable_board,Hint);
    printf("The cell should be set to %d .", board->board[x][y].solution_value);/*make sure with oved I should use validate*/

    return FALSE;
}

StatusType guess_h_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    /*ask Oved*/
    return FALSE;
}

StatusType num_s_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    int result;
    if (is_erroneous(board) == TRUE)
        return error_message(board_errorneus,Num_s);
    result = numOfSolutions(board);
    printf("The number of solutions of this board is %d",result);
    return FALSE;
}

StatusType autofill_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {

    return FALSE;
}

StatusType reset_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    ACTION * action;
    action = listUndo();
    while(action != NULL){
        do_set_by_action(*action,board,TRUE);
    }
    return FALSE;
}

StatusType exit_program_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    destroyList();
    destroyBoard(board);
    /*free files*/
    return FALSE;
}

StatusType check_range(int row, int column, int value, int size) {
    if (row < 0 || row > size || column < 0 || value < 0 || value > size+1)//////////CHECK IF NEEDS TO HAVE LOWER BOUND
    return FALSE;

    return TRUE;
}

