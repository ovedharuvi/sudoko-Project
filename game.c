
#include "game.h"


/*check_range checks if the parameters given in set command are valid to the current board.
 * return value : statusType. returns TRUE if the row,column and value in the correct range of the board.
*/
StatusType check_range(int row, int column, int value, int size);

/* do_set_by_action  executes the set order according to a given action from the doubly linked list.
 * no return value.
 * parameters : is_undo- True if undo command FALSE if redo command. */
void do_set_by_action(ACTION action, sudokoBoard *board, int is_undo);

/* fill_legal_values fills an array in the size of the board that represents if it's legal
 * to put the value i of a given cell. in place i there is representation of the value i+1. */
void fill_legal_values(int row, int column, sudokoBoard *board, StatusType *array);

/*checks if in a boolean array only one cell is set to 1
 * return value: 1 if true, else 0. */
int check_single_solution(StatusType *array, int boardSize);

/* fill_board is a function inside load function.
 * according to the file format parameters fill_board creates new sudokoBoard and assign in to the the parameter
 * and fills values by the board of the file.*/
StatusType fill_board(sudokoBoard **boardPtr, char *pString);

/*loads the board x and changes the MODE to SOLVE_MODE.
 * 'cmd' return value.
 * possible errors : file error in load.*/
StatusType solve_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode,
                     int paramNum);

/* Changes the MODE to EDIT_MODE and assign sudokoBoard * to the sudokoBoard **board given as a parameter. .
 parameters : * no parameter : creates 9*9 empty board and assign it to sudokoBoard **board
              * loads the board of the file x given and assign it to sudokoBoard **board.
 * possible errors : file error in load.*/
StatusType edit_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum);

/* changes the global variable MarkErrors to 1 or 0 according to the given parameter .
 * 'cmd' return value. : */
StatusType mark_errors_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode,
                           int paramNum);

/*prints the board and calls printBoard*/
StatusType print_board_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum);

/*sets value in the board according to the given parameters.
 * 'cmd' return value.
 *possible errors : incorrect range of parameters, fixed cell in SOLVE_MODE. */
StatusType set_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum);

/* calls validate from the solver and prints whether the board has a solution according to the returned value from solver.
 * 'cmd' return value
 * possible errors : board is erroneous,board is unsolvable, memory error, incorrect range of parameters */
StatusType validate_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum);

/*calls guess from the solver and updates the board accordingly.
 * 'cmd' return value
* possible errors : board is erroneous,board is unsolvable, memory error, incorrect range of parameters */
StatusType guess_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum);

/*calls generate from the solver and updates the board accordingly.
 * 'cmd' return value.
* possible errors : board is unsolvable, memory error, incorrect range of parameters */
StatusType generate_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum);

/* undo the last move was done by the user. calls to listUndo() from doubly linked.
 * 'cmd' return value.
 * possible errors : no moves left to undo.*/
StatusType undo_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode,
                    int paramNum);

/*redo the last move was done by the user. calls to listRedo() from doubly linked.
 * 'cmd' return value.
 * possible errors : no moves left to redo.*/
StatusType redo_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode,
                    int paramNum);

/*saves the board to the file x given according to the MODE.
 * 'cmd' return value.
 * possible errors : file error - the path wasn't found or invalid file format.*/
StatusType save_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode,
                    int paramNum);

/* prints to the user the solution value given by 'validate' from solver.
 * 'cmd' return value.
 * possible errors: board is erroneous,board is unsolvable, cell is fixed, non empty cell.*/
StatusType hint_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum);

/* calls guessHint from the solver.
 * 'cmd' return value.
 * possible errors : incorrect range of parameters.*/
StatusType guess_h_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum);

/* calls numOfSolutions from the solver and prints it to the user.
 * 'cmd' return value.
 * possible errors : board is erroneous. */
StatusType num_s_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum);

/*fills with set command the cells that has single value in board.
 * 'cmd' return value.
 * possible errors : memory error.*/
StatusType autofill_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum);

/* bring the board to it's first stage of values using the doubly linked list.
 * 'cmd' return value.*/
StatusType reset_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum);

/*returns EXIT which makes the main program flow to exit and release memory.*/
StatusType exit_program_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum);

/*loads board from the file path given as a paramter.
  * possible errors : file error - the path wasn't found or invalid file format.*/
sudokoBoard *load(char *path);

/*returns TRUE if all board is filled else FALSE*/
StatusType is_game_over(sudokoBoard *board_ptr);


CmdInfo CmdArray[ORDERS_NUM];
int MarkErrors;

void init_game() {
    MarkErrors = TRUE; /*global variable set to TRUE on INIT mode*/
    SetCmdArray();/* init the CMD Array*/
    printf("Welcome to Sudoku Game !\n To initialize new board you can enter 'edit'.\n"
           "To load a game you can enter 'edit' or 'solve' and then the path of your board.\n");

}

void SetTableInfo(CmdType cmdtype,
                  StatusType (*fun_ptr)(char **, sudokoBoard **, MODE *, int),
                  MODE mode, int paramNum, ParamType paramType, char *cmdName) {
    CmdArray[cmdtype].fun_ptr = fun_ptr;
    CmdArray[cmdtype].mode = mode;
    CmdArray[cmdtype].paramNum = paramNum;
    CmdArray[cmdtype].paramType = paramType;
    CmdArray[cmdtype].cmdName = cmdName;

}

void SetCmdArray() {
    SetTableInfo(SOLVE, &solve_cmd, INIT_MODE + EDIT_MODE + SOLVE_MODE, 1, String, "solve");
    SetTableInfo(EDIT, &edit_cmd, INIT_MODE + EDIT_MODE + SOLVE_MODE, 1, String, "edit");
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
    SetTableInfo(EXIT_GAME, &exit_program_cmd, INIT_MODE + EDIT_MODE + SOLVE_MODE, 0, None, "exit");
}

void make_board_equal(sudokoBoard *board_ptr, sudokoBoard *copy, CmdType cmdType) {
    int i, j, n, oldValue, newValue;
    int first_insert;

    first_insert = 1;

    n = board_ptr->boardSize;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            oldValue = board_ptr->board[i][j].value;
            newValue = copy->board[i][j].value;
            if (oldValue != newValue) {
                board_ptr->board[i][j].value = newValue;
                InsertAction(oldValue, newValue, i, j, first_insert ? FALSE : TRUE, cmdType);
                first_insert = 0;
            }
        }

    }
}

sudokoBoard *load(char *path) {
    StatusType status;
    FILE *file_ptr;
    char *buffer = 0;
    long length;
    sudokoBoard *board_ptr;
    file_ptr = fopen(path, "rb");

    if (file_ptr == NULL) {
        return NULL;
    }

    fseek(file_ptr, 0, SEEK_END);
    length = ftell(file_ptr);
    fseek(file_ptr, 0, SEEK_SET);
    buffer = (char *) malloc(length);
    if (buffer == NULL) {
        return NULL;
    }

    status = fread(buffer, 1, length, file_ptr);
    if (status == EOF) {
        return NULL;
    }

    fclose(file_ptr);

    status = fill_board(&board_ptr, buffer);
    if (status == FALSE)
        return NULL;

    myFree(buffer);


    return board_ptr;
}

StatusType fill_board(sudokoBoard **boardPtr, char *pString) {
    int i = 0, j, size, value = 0, status = 0, result = 0, n, m;
    char dot;
    char delimit[] = " \t\r\n\v\f";
    char *ptoken;
    sudokoBoard *newBoard;

    /*creating board*/

    ptoken = strtok(pString, delimit);
    if (ptoken == NULL) {
        return FALSE;
    }
    result += sscanf(ptoken, "%d", &m);

    ptoken = strtok(NULL, delimit);
    if (ptoken == NULL) {
        return FALSE;
    }
    result += sscanf(ptoken, "%d", &n);

    if (result != 2) {
        return FALSE;
    }

    newBoard = createBoard(m, n);
    if (newBoard == NULL) {
        return FALSE;
    }
    size = newBoard->boardSize;

    result = 0;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            ptoken = strtok(NULL, delimit);
            if (ptoken == NULL) {
                break;
            }

            status = sscanf(ptoken, "%d%c", &value, &dot);
            if (status > 0) {
                if(check_range(i,j,value,size)== FALSE){
                    return FALSE;
                }
                newBoard->board[i][j].value = value;
                result++;
                if (status == 2) {
                    if (dot != '.'){
                        return FALSE;
                    }
                    newBoard->board[i][j].is_fixed = TRUE;
                }
            }
            if (pString == '\0') {
                break;
            }

        }
    }

    if (result != size * size)
        return FALSE;

    *boardPtr = newBoard;
    return TRUE;
}


StatusType is_game_over(sudokoBoard *board_ptr) {
    int i, n, j;

    n = board_ptr->boardSize;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (board_ptr->board[i][j].value == 0) {
                return FALSE;
            }

        }

    }
    return TRUE;
}

StatusType solve_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum) {
    sudokoBoard *newBoard;
    newBoard = load(paramsArray[paramNum - 1]);/*first and only parameter of solve function - string of the path*/
    if (newBoard == NULL)/*check if load is valid*/
        return error_message(file_error, CmdArray[SOLVE]);
    *p_mode = SOLVE_MODE;


    /*start over board and doubly linked list in case they are initialized*/

    destroyBoard(*board);
    destroyList();/*maintain the doublylinkedlist*/
    *board = newBoard;
    print_board_cmd(paramsArray, board, p_mode, paramNum);

    return TRUE;
}

StatusType edit_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum) {
    sudokoBoard *newBoard;
    if (paramsArray[0] == '\0') {/*check condition, if no parameter*/
        newBoard = createBoard(DEFUALT_SIZE, DEFUALT_SIZE);
    } else {
        newBoard = load(
                paramsArray[paramNum - 1]);/*first and only parameter of solve function - string of the path*/
    }

    if (newBoard == NULL)/*check if load is valid*/
        return error_message(file_error, CmdArray[EDIT]);

    *p_mode = EDIT_MODE;

    /*start over board and doubly linked list in case they are initialized*/
    destroyBoard(*board);
    destroyList();/*maintain the doublylinkedlist*/

    *board = newBoard;

    print_board_cmd(paramsArray, board, p_mode, paramNum);
    return TRUE;
}

StatusType mark_errors_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum) {
    int param;

    param = atoi(paramsArray[0]);
    if (param == 0 || param == 1) {
        MarkErrors = param;
        return FALSE;
    }
    print_board_cmd(paramsArray, board, p_mode, paramNum);
    return error_message(incorrect_range, CmdArray[MARK]);
}


StatusType print_board_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum) {
    int mark_fixed = TRUE;
    int mark_errors;

    /*for unused parameters*/
    UNUSED (paramNum);
    UNUSED (paramsArray);

    mark_errors = MarkErrors;

    if (*p_mode == EDIT_MODE) {
        mark_errors = TRUE;
        mark_fixed = FALSE;
    }
    printBoard(FALSE, *board, mark_errors, mark_fixed);
    return FALSE;
}

StatusType set_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum) {
    int i, j, value, n;
    StatusType status;
    j = atoi(paramsArray[0]) - 1;/* -1 beacause the user enters in 1 base */
    i = atoi(paramsArray[1]) - 1;
    value = atoi(paramsArray[2]);
    n = (*board)->boardSize;

    /*check range of params is valid*/
    status = check_range(i, j, value, n);
    if (status == FALSE) {
        return error_message(incorrect_range, CmdArray[SET]);
    }

    /*check if fixed and solve mode*/
    if (*p_mode == SOLVE_MODE && (*board)->board[i][j].is_fixed == 1) {
        return error_message(fixed_cell, CmdArray[SET]);
    }

    /*maintains doubly linked list*/
    InsertAction((*board)->board[i][j].value, value, i, j, FALSE, SET);

    /*Set action itself*/
    (*board)->board[i][j].value = value;
    print_board_cmd(paramsArray, board, p_mode, paramNum);


    return TRUE;
}

StatusType check_game_over(sudokoBoard *board, CmdInfo cmdInfo) {
    StatusType status;

    status = is_game_over(board);

    if (status == FALSE) {
        return FALSE;
    }

    if (is_erroneous(board)) {
        return error_message(board_erroneous, cmdInfo);/*return error of erroneous*/
    } else {
        printf("Congratulations !!! You solved the board! \n");


    }
    return TRUE;
}


StatusType validate_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum) {
    int result;

    /*for unused parameters*/
    UNUSED (paramNum);
    UNUSED (paramsArray);
    UNUSED (p_mode);

    result = validate(*board);
    printf(result ? "The board is solvable.\n" : "The board is unsolvable.\n");
    return FALSE;
}


StatusType guess_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum) {
    sudokoBoard *copy;
    float f;

    f = atof(paramsArray[0]);

    if (f < 0 || f > 1) {
        return error_message(incorrect_range, CmdArray[GUESS]);
    }

    if (is_erroneous(*board) == TRUE)
        return error_message(board_erroneous, CmdArray[GUESS]);

    copy = guess(*board, f);

    if (copy == *board) {
        return error_message(unsolvable_board, CmdArray[GUESS]);
    }
    if (copy == NULL) {
        return error_message(memory_error, CmdArray[GUESS]);
    }
    make_board_equal(*board, copy, GUESS);
    destroyBoard(copy);
    print_board_cmd(paramsArray, board, p_mode, paramNum);


    return TRUE;
}

StatusType generate_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum) {
    int x, y;
    sudokoBoard *copy;
    StatusType status;

    x = atoi(paramsArray[0]);
    y = atoi(paramsArray[1]);
    status = isEmptySmallerThanLegalVal(*board, x);
    if (status == TRUE) {
        return error_message(incorrect_range, CmdArray[GENERATE]);
    }
    if (is_erroneous(*board) == TRUE)
        return error_message(board_erroneous, CmdArray[GENERATE]);

    copy = generate(*board, x, y);

    if (copy == *board) {
        return error_message(unsolvable_board, CmdArray[GENERATE]);
    }
    if (copy == NULL) {
        return error_message(memory_error, CmdArray[GENERATE]);
    }
    make_board_equal(*board, copy, GENERATE);
    destroyBoard(copy);
    print_board_cmd(paramsArray, board, p_mode, paramNum);
    return TRUE;
}

StatusType undo_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum) {
    ACTION *action_ptr = NULL;
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
                do_set_by_action(*action_ptr, *board, TRUE);
                action_ptr = listUndo();
            }
            do_set_by_action(*action_ptr, *board, TRUE);
            break;
        case SET:
            do_set_by_action(*action_ptr, *board, TRUE);
            break;
        default:
            return FALSE;
    }

    print_board_cmd(paramsArray, board, p_mode, paramNum);
    return TRUE;
}

void do_set_by_action(ACTION action, sudokoBoard *board, int is_undo) {
    int value, i, j;
    i = action.row;
    j = action.column;
    if (is_undo == TRUE) {
        value = action.oldValue;
    } else {
        value = action.newValue;
    }
    board->board[i][j].value = value;

}


StatusType redo_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum) {
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
            do_set_by_action(*action_ptr, *board, FALSE);
            action_ptr = listRedo();
            if (action_ptr == NULL) {
                break;
            }
            while (action_ptr->insertedByComputer == TRUE) {
                do_set_by_action(*action_ptr, *board, FALSE);
                action_ptr = listRedo();
                if (action_ptr == NULL) {
                    break;
                }
            }
            break;
        case SET:
            do_set_by_action(*action_ptr, *board, FALSE);
            break;
        default:
            return FALSE;
    }
    print_board_cmd(paramsArray, board, p_mode, paramNum);
    return TRUE;
}

StatusType save_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum) {
    char *path;
    FILE *file_ptr;
    int i, j, n, bool;

    /*for unsued parameteres*/
    UNUSED(paramsArray);
    UNUSED(paramNum);

    /*can't save an erroneous board in EDITs mode*/
    if (*p_mode == EDIT_MODE) {
        if (is_erroneous(*board) == TRUE) {
            return error_message(board_erroneous, CmdArray[SAVE]);
        }
        if (validate(*board) == FALSE) {
            return error_message(unsolvable_board, CmdArray[SAVE]);
        }
    }

    path = paramsArray[0];
    n = (*board)->boardSize;
    file_ptr = fopen(path, "w");
    if (file_ptr == NULL) {
        return error_message(file_error, CmdArray[SAVE]);
    }

    fprintf(file_ptr, "%d %d \n", (*board)->heightOfBlock, (*board)->widthOfBlock);
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            bool = *p_mode == EDIT_MODE && ((*board)->board[i][j].value != 0);
            if (bool || (*board)->board[i][j].is_fixed) {
                fprintf(file_ptr, "%d.", (*board)->board[i][j].value);
            } else {
                fprintf(file_ptr, "%d", (*board)->board[i][j].value);
            }
            if (j != n - 1) {
                fprintf(file_ptr, " ");
            } else {
                fprintf(file_ptr, "\n");
            }
        }

    }
    fclose(file_ptr);
    printf("Board saved to: %s\n", path);

    return FALSE;
}

StatusType hint_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum) {
    int x, y;

    /*for unused parameters*/
    UNUSED (paramNum);
    UNUSED (p_mode);

    if (is_erroneous(*board) == TRUE)
        return error_message(board_erroneous, CmdArray[HINT]);
    x = atoi(paramsArray[0]) - 1;
    y = atoi(paramsArray[1]) - 1;
    if ((*board)->board[x][y].is_fixed)
        return error_message(fixed_cell, CmdArray[HINT]);
    if ((*board)->board[x][y].value != 0)
        return error_message(non_empty_cell, CmdArray[HINT]);
    if (validate(*board) != TRUE)
        return error_message(unsolvable_board, CmdArray[HINT]);
    printf("The cell should be set to %d .\n",
           (*board)->board[x][y].solution_value);

    return FALSE;
}

StatusType guess_h_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum) {
    int i, j, n;
    StatusType status;

    /*for unused parameters*/
    UNUSED (paramNum);
    UNUSED (p_mode);


    j = atoi(paramsArray[0]) - 1;
    i = atoi(paramsArray[1]) - 1;
    n = (*board)->boardSize;
    /*check range of params is valid*/

    status = check_range(i, j, i, n);
    if (status == FALSE) {
        return error_message(incorrect_range, CmdArray[GUESS_H]);
    }

    guessHint(*board, i, j);
    return TRUE;
}

StatusType num_s_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum) {
    int result;

    /*for unused parameters*/
    UNUSED (paramNum);
    UNUSED (p_mode);
    UNUSED (paramsArray);

    if (is_erroneous(*board) == TRUE)
        return error_message(board_erroneous, CmdArray[NUM_S]);
    result = numOfSolutions(*board);
    printf("The board has %d different solutions. ", result);
    return FALSE;
}

StatusType autofill_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum) {
    sudokoBoard *copy;
    int i, j, n, value, firstInsert;
    StatusType *statusArray;
    statusArray = (StatusType *) malloc(sizeof(StatusType) * ((*board)->boardSize));
    n = (*board)->boardSize;
    firstInsert = 1;
    copy = copyBoard(*board);
    if (copy == NULL) {
        return error_message(memory_error, CmdArray[AUTOFILL]);
    }
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            fill_legal_values(i, j, copy, statusArray);
            value = check_single_solution(statusArray, n);
            if (value != 0) {
                InsertAction((*board)->board[i][j].value, value, i, j, (firstInsert ? FALSE : TRUE), AUTOFILL);
                firstInsert = 0;                                                   /*maintain doubly linked list*/
                (*board)->board[i][j].value = value;

            }
        }
    }

    destroyBoard(copy);


    myFree(statusArray);

    print_board_cmd(paramsArray, board, p_mode, paramNum);
    return TRUE;
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
        array[i] = checkIfValid(board, i + 1, row, column);
    }
}

StatusType reset_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum) {
    ACTION *action;
    action = listUndo();
    while (action != NULL) {
        do_set_by_action(*action, *board, TRUE);
        action = listUndo();
    }
    print_board_cmd(paramsArray, board, p_mode, paramNum);
    return TRUE;
}


StatusType exit_program_cmd(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum) {

    /*for unused parameters*/

    UNUSED (paramNum);
    UNUSED (p_mode);
    UNUSED (paramsArray);
    UNUSED(board);

    return EXIT;
}

StatusType check_range(int row, int column, int value, int size) {
    if (row < 0 || row > size || column < 0 || value < 0 ||
        value > size + 1)
        return FALSE;

    return TRUE;
}

void exit_game(sudokoBoard *board_ptr, int is_exit_program) {

    destroyBoard(board_ptr);
    destroyList();
    if (is_exit_program == TRUE) {
        printf("Exiting...\n");
    }

}

void maintain_erroneous(sudokoBoard *board) {
    int i,j,n;
    StatusType status;
    n = board->boardSize;


    for (i = 0 ; i < n; i++){
        for(j = 0 ; j < n ; j++){
            status = checkIfValid(board,board->board[i][j].value,i,j);
            if (status){
                board->board[i][j].is_erroneus = 0;
            }
            else{
                board->board[i][j].is_erroneus = 1;
            }
        }
    }
}

