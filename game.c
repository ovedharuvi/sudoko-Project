//
// Created by oved on 02/08/2019.
//

#include "game.h"

/*returns TRUE if the row,column and value in the correct range of the board */
StatusType check_range(int row, int column, int value, int size);

/* */
void do_set_by_action(ACTION action, sudokoBoard *board, int is_undo);

void maintain_erroneous(int row, int column, int value, sudokoBoard *board);

void fill_legal_values(int row, int column, sudokoBoard *board, StatusType *array);

int check_single_solution(StatusType *array, int boardSize);

StatusType get_size(int *p_m, int *p_n, char **pString);

StatusType fill_board(sudokoBoard *boardPtr, char *pString);

void SetTableInfo(CmdType cmdtype,
                  StatusType (*fun_ptr)(char **, sudokoBoard *, MODE *, int),
                  MODE mode, int paramNum, ParamType paramType, char *cmdName) {
    CmdArray[cmdtype].fun_ptr = fun_ptr;
    CmdArray[cmdtype].mode = mode;
    CmdArray[cmdtype].paramNum = paramNum;
    CmdArray[cmdtype].paramType = paramType;
    CmdArray[cmdtype].cmdName = cmdName;

}

void SetCmdArray() {
    SetTableInfo(SOLVE, &solve_cmd, INIT_MODE + EDIT_MODE + SOLVE_MODE, 1, String, "solve");
    SetTableInfo(EDIT, &edit_cmd, INIT_MODE + EDIT_MODE + SOLVE_MODE, 1, String + None, "edit");
    SetTableInfo(MARK, &mark_errors_cmd, SOLVE_MODE, 1, Integer, "mark_errors");
    SetTableInfo(PRINT, &print_board_cmd, EDIT_MODE + SOLVE_MODE, 0, None, "print_board");
    SetTableInfo(SET, &set_cmd, EDIT_MODE + SOLVE_MODE, 3, Integer, "set");
    SetTableInfo(VALIDATE, &validate_cmd, EDIT_MODE + SOLVE_MODE, 0, None, "validate");
    SetTableInfo(GUESS, &guess_cmd, SOLVE_MODE, 1, Float, "guess");
    SetTableInfo(GENERATE, &generate_cmd, EDIT_MODE, 2, Integer, "generate");
    SetTableInfo(UNDO, &undo_cmd, EDIT_MODE + SOLVE_MODE, 0, None, "undo");
    SetTableInfo(REDO, &redo_cmd, EDIT_MODE + SOLVE_MODE, 0, None, "redo");
    SetTableInfo(SAVE, &save_cmd, EDIT_MODE + SOLVE_MODE, 1, String, "save");
    SetTableInfo(HINT, &hint_cmd, SOLVE_MODE, 2, Integer, "hint");
    SetTableInfo(GUESS_H, &guess_h_cmd, SOLVE_MODE, 2, Integer, "guess_hint");
    SetTableInfo(NUM_S, &num_s_cmd, EDIT_MODE + SOLVE_MODE, 0, None, "num_solutions");
    SetTableInfo(AUTOFILL, &autofill_cmd, SOLVE_MODE, 0, None, "autofill");
    SetTableInfo(RESET, &reset_cmd, EDIT_MODE + SOLVE_MODE, 0, None, "reset");
    SetTableInfo(EXIT, &exit_program_cmd, INIT_MODE + EDIT_MODE + SOLVE_MODE, 0, None, "exit");
}

/*
CmdInfo Solve ={solve_cmd, INIT_MODE + EDIT_MODE + SOLVE_MODE , 1, String, "solve"};
CmdInfo Edit ={&edit_cmd, INIT_MODE + EDIT_MODE + SOLVE_MODE, 1, String + None, "edit"};
CmdInfo Mark ={&mark_errors_cmd, SOLVE_MODE, 1, Integer, "mark_errors"};
CmdInfo Print ={&print_board_cmd, EDIT_MODE + SOLVE_MODE, 0, None, "print_board"};
CmdInfo Set ={&set_cmd, EDIT_MODE + SOLVE_MODE, 3, Integer, "set"};
CmdInfo Validate ={&validate_cmd, EDIT_MODE + SOLVE_MODE, 0, None, "validate"};
CmdInfo Guess ={&guess_cmd, SOLVE_MODE, 1, Float, "guess"};
CmdInfo Generate ={&generate_cmd, EDIT_MODE, 2, Integer, "generate"};
CmdInfo Undo ={&undo_cmd, EDIT_MODE + SOLVE_MODE, 0, None, "undo"};
CmdInfo Redo ={&redo_cmd, EDIT_MODE + SOLVE_MODE, 0, None, "redo"};
CmdInfo Save ={&save_cmd, EDIT_MODE + SOLVE_MODE, 1, String, "save"};    //in edit mode :validate, all fixed cells
CmdInfo Hint ={&hint_cmd, SOLVE_MODE , 2, Integer, "hint"};
CmdInfo Guess_h ={&guess_h_cmd, SOLVE_MODE, 2, Integer, "guess_hint"};
CmdInfo Num_s ={&num_s_cmd, EDIT_MODE + SOLVE_MODE, 0, None, "num_solutions"};
CmdInfo Autofill ={&autofill_cmd, SOLVE_MODE, 0, None, "autofill"};
CmdInfo Reset_Game ={&reset_cmd, EDIT_MODE + SOLVE_MODE, 0, None, "reset"};
CmdInfo Exit ={&exit_program_cmd, INIT_MODE + EDIT_MODE + SOLVE_MODE, 0, None, "exit"};
*/
void make_board_equal(sudokoBoard *board_ptr, sudokoBoard *copy, CmdType cmdType) {
    int i, j = 0, n, oldValue, newValue;

    n = board_ptr->boardSize;
    for (i = 0; i < n; i++) {
        oldValue = board_ptr->board[i][j].value;
        newValue = copy->board[i][j].value;
        if (oldValue != newValue) {
            board_ptr->board[i][j].value = newValue;
            InsertAction(oldValue, newValue, i, j, FALSE, cmdType);
        }
    }

}

sudokoBoard *load(char *path) {
    int m, n;
    StatusType status;
    FILE *file_ptr;
    char **buffer = 0;
    long length;
    sudokoBoard *board_ptr;
    file_ptr = fopen(path, "rb");

    if (file_ptr == NULL) {
        return NULL;
    }

    fseek(file_ptr, 0, SEEK_END);
    length = ftell(file_ptr);
    fseek(file_ptr, 0, SEEK_SET);
    buffer = malloc(length);
    if (*buffer == NULL) {
        return NULL;
    }

    status = fread(*buffer, 1, length, file_ptr);
    if (status == EOF) {
        return NULL;
    }

    fclose(file_ptr);

    status = get_size(&m, &n, buffer);
    if (status == FALSE)
        return NULL;
    board_ptr = createBoard(m, n, FALSE);
    status = fill_board(board_ptr, *buffer);
    if (status == FALSE)
        return NULL;


    free(buffer);

    return board_ptr;
}

StatusType fill_board(sudokoBoard *boardPtr, char *pString) {
    int i, j, n, value = 0, status = 0;
    char dot;
    char delimit[] = " \t\r\n\v\f";
    char *ptoken;
    int result = 0;

    n = boardPtr->boardSize;
    ptoken = strtok(pString, delimit);
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) {
            if (ptoken != NULL) {
                status = sscanf(ptoken, "%d%c", &value, &dot);
                if (status > 0) {
                    boardPtr->board[i][j].value = value;
                    result++;
                    if (status == 2) {
                        boardPtr->board[i][j].is_fixed = TRUE;
                    }
                }
                ptoken = strtok(NULL, delimit);
            } else {
                break;
            }
        }
    if (result != n * n)
        return FALSE;
    return TRUE;
}

StatusType get_size(int *p_m, int *p_n, char **pString) {
    char delimit[] = " \t\r\n\v\f";
    char *ptoken;
    int result = 0;

    if ((ptoken = strtok(*pString, delimit)) != NULL)
        result += sscanf(ptoken, "%d", p_m);
    if ((ptoken = strtok(NULL, delimit)) != NULL)
        result += sscanf(ptoken, "%d", p_n);


    if (result == 2) {
        *pString = ptoken;
        return TRUE;
    }

    return FALSE;
}

StatusType is_game_over(sudokoBoard *board_ptr) {
    int i, n, j = 0;

    n = board_ptr->boardSize;
    for (i = 0; i < n; i++) {
        if (board_ptr->board[i][j].value == 0) {
            return FALSE;
        }
    }
    return TRUE;
}

StatusType solve_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    board = load(paramsArray[paramNum - 1]);/*first and only parameter of solve function - string of the path*/
    if (board == NULL)/*check if load is valid*/
        return FALSE;
    *p_mode = SOLVE_MODE;
    destroyList();/*maintain the doublylinkedlist*/
    print_board_cmd(paramsArray, board, p_mode, paramNum);

    return FALSE;
}

StatusType edit_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    if (paramsArray[0][0] == '\0') {///////check condition, if no parameter
        board = createBoard(DEFUALT_SIZE, DEFUALT_SIZE, FALSE);
    } else {
        board = load(paramsArray[paramNum - 1]);/*first and only parameter of solve function - string of the path*/
    }

    if (board == NULL)/*check if load is valid*/
        return FALSE;

    *p_mode = EDIT_MODE;
    destroyList();/*maintain the doublylinkedlist*/
    print_board_cmd(paramsArray, board, p_mode, paramNum);
    return FALSE;
}

StatusType mark_errors_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    int param;

    /*for unused parameters*/
    (void) paramNum;
    (void) p_mode;

    param = atoi(paramsArray[0]);
    if (param == 0 || param == 1) {
        board->markErrors = param;
        return TRUE;
    }
    return error_message(incorrect_range, CmdArray[MARK]);
}


StatusType print_board_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    int mark_errors;

    /*for unused parameters*/
    (void) paramNum;
    (void) paramsArray;

    mark_errors = board->markErrors;
    if (*p_mode == EDIT_MODE) {
        mark_errors = TRUE;
    }
    printBoard(FALSE, board, mark_errors);
    return FALSE;
}

StatusType set_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    int i, j, value, n;
    StatusType status;
    i = atoi(paramsArray[0]) - 1;/* -1 beacause the user enters in 1 base */
    j = atoi(paramsArray[1]) - 1;
    value = atoi(paramsArray[2]);
    n = board->boardSize;

    /*check range of params is valid*/
    status = check_range(i, j, value, n);
    if (status == FALSE) {
        return error_message(incorrect_range, CmdArray[SET]);
    }

    /*check if fixed and solve mode*/
    if (*p_mode == SOLVE_MODE && board->board[i][j].is_fixed == 1) {
        return error_message(fixed_cell, CmdArray[SET]);
    }

    maintain_erroneous(i, j, value, board);

    /*maintain doubly linked list*/
    InsertAction(board->board[i][j].value, value, i, j, 0, SET);

    /*Set action itself*/
    board->board[i][j].value = value;
    print_board_cmd(paramsArray, board, p_mode, paramNum);
    status = is_game_over(board);


    if (status == TRUE) {
        if (is_erroneous(board) == TRUE) {
            return error_message(board_errorneus, CmdArray[SET]);/*return error of erroneous*/
        } else exit_program();/////////////////////release everything and back to INIT - new function

    }

    return status;
}

/* maintain the cells field "is_erroneous*/
void maintain_erroneous(int row, int column, int value, sudokoBoard *board) {
    checkIfValid(board, value, row, column, TRUE);

}

StatusType validate_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    int result;

    /*for unused parameters*/
    (void) paramNum;
    (void) paramsArray;
    (void) p_mode;



    result = validate(board);
    printf(result != SOLVABLE ? "The board is unsolvable." : "The board is solvable.");
    return FALSE;
}


StatusType guess_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    sudokoBoard *copy;
    float f;

    f = atof(paramsArray[0]);

    if (f < 0 || f > 1) {
        return error_message(incorrect_range, CmdArray[GUESS]);
    }

    if (is_erroneous(board) == TRUE)
        return error_message(board_errorneus, CmdArray[VALIDATE]);

   copy = guess(board, f);//// board and not int ?
    make_board_equal(board, copy, GUESS);
    print_board_cmd(paramsArray, board, p_mode, paramNum);


    return FALSE;
}

StatusType generate_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    int x, y;
    sudokoBoard * copy;
    StatusType status;

    x = atoi(paramsArray[0]);
    y = atoi(paramsArray[1]);
   status = isEmptySmallerThenLegalVal(board,x);
   if (status == FALSE){
       return error_message(incorrect_range,CmdArray[GENERATE]);
   }
    copy = generate(board, x, y);
    print_board_cmd(paramsArray, board, p_mode, paramNum);
    return FALSE;
}

StatusType undo_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    ACTION *action_ptr;
    CmdType cmd;
    action_ptr = listUndo();
    if (action_ptr == NULL)
        return error_message(no_moves_error, CmdArray[UNDO]);
    cmd = action_ptr->command;
    switch (cmd) {
        case GENERATE:
        case AUTOFILL:
        case GUESS:
            while (action_ptr->insertedByComputer == TRUE) {
                do_set_by_action(*action_ptr, board, TRUE);
                action_ptr = listUndo();
            }
            break;
        case SET:
            do_set_by_action(*action_ptr, board, TRUE);
            break;
        default:
            return error_message(invalid_move, CmdArray[cmd]);
    }

    print_board_cmd(paramsArray, board, p_mode, paramNum);
    return FALSE;
}

void do_set_by_action(ACTION action, sudokoBoard *board, int is_undo) {
    int value, i, j;
    i = action.row;
    j = action.column;
    if (is_undo == TRUE) {
        value = action.oldValue;
    } else { value = action.newValue; }
    board->board[i][j].value = value;

}


StatusType redo_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    ACTION *action_ptr;
    CmdType cmd;
    action_ptr = listRedo();
    if (action_ptr == NULL)
        return error_message(no_moves_error, CmdArray[REDO]);
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
    print_board_cmd(paramsArray, board, p_mode, paramNum);
    return FALSE;
}

StatusType save_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    /*for unused parameters*/
    (void) paramNum;
    (void) p_mode;
    (void) paramsArray;
    (void) board;

    return FALSE;
}

StatusType hint_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    int x, y;

    /*for unused parameters*/
    (void) paramNum;
    (void) p_mode;

    if (is_erroneous(board) == TRUE)
        return error_message(board_errorneus, CmdArray[HINT]);
    x = atoi(paramsArray[0]) - 1;
    y = atoi(paramsArray[1]) - 1;
    if (board->board[x][y].is_fixed)
        return error_message(fixed_cell, CmdArray[HINT]);
    if (board->board[x][y].value != 0)
        return error_message(non_empty_cell, CmdArray[HINT]);
    if (validate(board) != SOLVABLE)
        return error_message(unsolvable_board, CmdArray[HINT]);
    printf("The cell should be set to %d .",
           board->board[x][y].solution_value);/*make sure with oved I should use validate*/

    return FALSE;
}

StatusType guess_h_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    /*ask Oved*/
    int i, j, n;
    StatusType status;

    /*for unused parameters*/
    (void) paramNum;
    (void) p_mode;


    i = atoi(paramsArray[0]) - 1;
    j = atoi(paramsArray[1]) - 1;
    n = board->boardSize;
    /*check range of params is valid*/

    status = check_range(i, j, 1, n);
    if (status == FALSE) {
        return error_message(incorrect_range, CmdArray[GUESS_H]);
    }

    guessHint(board, i, j);
    return FALSE;
}

StatusType num_s_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    int result;

    /*for unused parameters*/
    (void) paramNum;
    (void) p_mode;
    (void) paramsArray;

    if (is_erroneous(board) == TRUE)
        return error_message(board_errorneus, CmdArray[NUM_S]);
    result = numOfSolutions(board);
    printf("The number of solutions of this board is %d", result);
    return FALSE;
}

StatusType autofill_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    sudokoBoard *copy;
    int i, j, n, value;
    StatusType statusArray[MAX_BOARD_SIZE];
    n = board->boardSize;
    copy = copyBoard(board);

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            fill_legal_values(i, j, copy, statusArray);
            value = check_single_solution(statusArray, n);
            if (value != 0) {
                board->board[i][j].value = value;
            }
        }
    }
    print_board_cmd(paramsArray, board, p_mode, paramNum);
    return FALSE;
}

int check_single_solution(StatusType *array, int boardSize) {
    int i, result = 0, value = 0;
    for (i = 0; i < boardSize && result < 2; i++) {
        if (array[i] == TRUE) {
            result++;
            value = i + 1;
        }
    }
    if (result == 1)
        return value;
    else { return 0; }
}

void fill_legal_values(int row, int column, sudokoBoard *board, StatusType *array) {
    int i, n;
    n = board->boardSize;
    for (i = 0; i < n; i++) {
        array[i] = checkIfValid(board, i + 1, row, column, FALSE);
    }
}

StatusType reset_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {
    ACTION *action;
    action = listUndo();
    while (action != NULL) {
        do_set_by_action(*action, board, TRUE);
    }
    print_board_cmd(paramsArray, board, p_mode, paramNum);
    return FALSE;
}

StatusType exit_program_cmd(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum) {

    /*for unused parameters*/
    (void) paramNum;
    (void) p_mode;
    (void) paramsArray;

    destroyList();
    destroyBoard(board);
    /*free files*/
    return FALSE;
}

StatusType check_range(int row, int column, int value, int size) {
    if (row < 0 || row > size || column < 0 || value < 0 ||
        value > size + 1)//////////CHECK IF NEEDS TO HAVE LOWER BOUND
        return FALSE;

    return TRUE;
}

