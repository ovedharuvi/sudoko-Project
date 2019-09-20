

#include "parser.h"

StatusType get_line(char *pString);

StatusType check_param_num(int cmd_index, int num_params_given, CmdInfo *cmdArray);

StatusType check_valid_params(char *paramArray[], CmdInfo cmdInfo);

StatusType do_order(CmdInfo cmdInfo, MODE *p_mode, int paramNum, char *paramsArray[], sudokoBoard **p_board);

StatusType get_cmd(char *text, CmdInfo *cmdArray);

StatusType get_mode(int index, MODE Mode);

int get_params(char *text, char **paramsArray);

int
str_compare(const char *first, char *second);/*returns 0 if the second string is a prefix of the first string, else 1*/







/*void string_to_array(char textPtr[256], char *string, int size);*/

StatusType check_valid_params(char *paramArray[], CmdInfo cmdInfo) {
    int i, t, m;
    char n;
    int f;
    ParamType paramType;
    int param_num = cmdInfo.paramNum;


    for (i = 0; i < param_num; i++) {
        /* %n returns the number of bytes consumed by sscanf, so if cast to int of strlen equal to n
         * it means that there no bytes left after %d */
        if ((sscanf(paramArray[i], "%d%c", &t, &n) == 1) && (sscanf(paramArray[i], "%d", &t) == 1)) {
            paramType = Integer;

        } else if (sscanf(paramArray[i], "%d%c%d", &f, &n, &m) == 3 && (n == '.')) {
            paramType = Float;
        } else paramType = String;


        if (cmdInfo.paramType != paramType)
            return FALSE;
    }

    return TRUE;
}

StatusType check_param_num(int cmd_index, int num_params_given, CmdInfo *cmdArray) {
    if ((CmdType) cmd_index == EDIT && num_params_given == 0)
        return TRUE;

    if (cmdArray[cmd_index].paramNum == num_params_given)
        return TRUE;
    else return FALSE;
}


StatusType get_line(
        char *pString) {/*returns FALSE if the user entered more than 256 characters, TRUE if valid 256 characters, EXIT if exit by the user*/
    int c = 0, i = 0;
    StatusType status;
    printf("Please enter a command: ");
    while (1) {
        c = getchar();


        if (c == '\n') {/*command ends with enter from stdin*/
            status = TRUE;
            break;
        } else if (c == EOF) {
            status = EXIT;
            break;
        } else if (i == BUFFERSIZE) {
            fflush(stdin);
            return error_message(line_limit, CmdArray[EDIT]);
        }

        pString[i] = (char) c;

        i++;

    }
    fflush(stdin);
    return status;
}


StatusType get_cmd(char *text, CmdInfo *cmdArray) {
    int i;
    char *pcommand;
    char delimit[] = " \t\r\n\v\f";

    pcommand = strtok(text, delimit);

    if (pcommand == NULL)
        return (int) INVALID;


    for (i = 0; i < ORDERS_NUM; i++) {
        if (str_compare(cmdArray[i].cmdName, pcommand) == TRUE) {
            return i;
        }
    }
    return (int) INVALID;
}

/*
void string_to_array(char textPtr[256], char *string, int size) {
    int i;
    for (i = 0; i<size; i++){
        textPtr[i] = string[i];
    }
}
*/
StatusType get_mode(int index, MODE Mode) {
    /*SOLVE = 0,EDIT = 1,MARK = 2, PRINT = 3, SET = 4, VALIDATE = 5, GUESS = 6, GENERATE = 7, UNDO = 8,
     REDO = 9, SAVE = 10, HINT = 11, GUESS_H = 12, NUM_S = 13, AUTOFILL = 14, RESET = 15, EXIT_GAME = 16, INVALID =17}CmdType*/
    if ((CmdArray[index].mode) == SOLVE_MODE + EDIT_MODE + INIT_MODE) {
        return TRUE;
    }

    if ((CmdArray[index].mode) == Mode) {
        return TRUE;
    }

    if (CmdArray[index].mode == Mode + EDIT_MODE) {
        return TRUE;
    }

    if (CmdArray[index].mode == Mode + SOLVE_MODE) {
        return TRUE;
    }
    return FALSE;

}


int get_params(char *text, char **paramsArray) {
    char delimit[] = " \t\r\n\v\f";
    int i = 0;
    int result = 0;
    char *ptoken;/*the pointer to be returned by strtok*/

    while (i < MAX_PARAM) {
        ptoken = strtok(NULL, delimit);
        if (ptoken == NULL) {
            break;
        }
        paramsArray[i] = ptoken;
        result++;
        i++;
        if (*text == '\0') {
            break;
        }

    }

    return result;
}

StatusType do_order(CmdInfo cmdInfo, MODE *p_mode, int paramNum, char **paramsArray, sudokoBoard **p_board) {
    StatusType status;

   cmdInfo.fun_ptr(paramsArray, p_board, p_mode, paramNum);
    status = check_game_over(*p_mode, *p_board,cmdInfo );
    return status;
}

int str_compare(const char *first, char *second) {
    int i;
    for (i = 0; first[i] == second[i]; i++) {
        if (first[i] == '\0') {
            return TRUE;
        }
    }
    return FALSE;
}


StatusType order(sudokoBoard **board_ptr, MODE *p_mode) {
    char text[BUFFERSIZE] = {0};
    StatusType status;
    int cmd_index, param_num;
    char paramsArray[MAX_PARAM][BUFFERSIZE] = {{0}};

    status = get_line((char *) text);

    if (status != TRUE) {/*makes the flow exit the game or restart the order*/
        return status;
    }
    cmd_index = get_cmd(text, CmdArray);
    if (cmd_index == (int) INVALID) {
        return error_message(invalid_cmd, CmdArray[SOLVE]);
    }
    status = get_mode(cmd_index, *p_mode);/*checks whether this command is valid if cmd is edit or solve */
    if (status == FALSE) {
        return error_message(invalid_mode, CmdArray[cmd_index]);
    }
    param_num = get_params((char *) text, (char **) paramsArray);/*returns num of parameters given (above 3 it's 4)
 * and put the parameters in the variables*/
    status = check_param_num(cmd_index, param_num, CmdArray);/*checks if the num of parameters given matches the command.
 * if matches - returns TRUE. else returns FALSE */
    if (status == FALSE) {
        return error_message(invalid_param_num, CmdArray[cmd_index]);
    }
    if (param_num > 0) {
        status = check_valid_params((char **) paramsArray, CmdArray[cmd_index]);/*checks if the type of parameters given matches the command.
 * if matches - returns TRUE. else returns FALSE and prints error_message message.*/
        if (status == FALSE) {
            return error_message(invalid_param_type, CmdArray[cmd_index]);
        }
    }
    status = do_order(CmdArray[cmd_index], p_mode, param_num, (char **) paramsArray,
                      board_ptr);/*checks inner validation inside any function. returns TRUE if game is over else FALSE*/

    return status;
}



