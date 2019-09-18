

#include "new_parser.h"

StatusType get_line(char *pString);

StatusType check_param_num(int cmd_index, int num_params_given, CmdInfo *cmdArray);

StatusType check_valid_params(char *paramArray[], CmdInfo cmdInfo);

StatusType do_order(CmdInfo cmdInfo, MODE *p_mode, int paramNum, char *paramsArray[], sudokoBoard *p_board);

StatusType get_cmd(char *text, CmdInfo *cmdArray);

StatusType get_mode(int index, MODE Mode);

int get_params(char *text, char **paramsArray);

int
str_compare(const char *first, char *second);/*returns 0 if the second string is a prefix of the first string, else 1*/







/*void string_to_array(char textPtr[256], char *string, int size);*/

StatusType check_valid_params(char *paramArray[], CmdInfo cmdInfo) {
    int i, n, t;
    float f;
    ParamType paramType;
    int param_num = cmdInfo.paramNum;

    for (i = 0; i < param_num; i++) {
        /* %n returns the number of bytes consumed by sscanf, so if cast to int of strlen equal to n
         * it means that there no bytes left after %d */
        if ((sscanf(paramArray[i], "%d%n", &t, &n)) == 1 && n == (int) strlen(paramArray[i])) {
            paramType = Integer;

        } else if (sscanf(paramArray[i], "%f", &f) == 1) {
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

   /* fflush(stdin);*/
        while (1) {
        c = getchar();
       printf("\n%c\n", c);


        if (c == '\n') {/*command ends with enter from stdin*/
             printf("end\n");
            status =  TRUE;
            break;
        }

        else if (c == EOF) {
            status =  EXIT;
            break;
        }


        else if (i == BUFFERSIZE) {
            fflush(stdin);
            return error_message(line_limit, CmdArray[EDIT]);
        }

        pString[i] = (char) c;

        i++;
        printf("i is = %d",i);

    }
        fflush(stdin);
        return status;
}


StatusType get_cmd(char *text, CmdInfo *cmdArray) {
    int i;
    char *pcommand;
   /* char text_ptr[BUFFERSIZE];*/
    char delimit[] = " \t\r\n\v\f";

    printf("text is %s\n",text);
    /*
    string_to_array(text_ptr,*text,BUFFERSIZE);
    printf("text_ptr is %s\n",text_ptr);
     */
    pcommand = strtok(text, delimit);
    printf("pcommand is %s\n",pcommand);

    if (pcommand == NULL)
        return (int) INVALID;
    /*
    *text = pcommand;*/ /*let text point to the place after the first token by reference*/
    strcpy(text,pcommand);
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
    return (int) CmdArray[index].mode == (int) Mode ? TRUE : FALSE;
}


int get_params(char *text, char **paramsArray) {
    char delimit[] = " \t\r\n\v\f";
    int i = 0;
    int result = 0;
    char *ptoken;/*the pointer to be returned by strtok*/
    while (i < MAX_PARAM) {

        if (i == 0) {
            ptoken = strtok(text, delimit);
        } else {
            ptoken = strtok(NULL, delimit);
        }

        if (ptoken != NULL) {
            strcpy(paramsArray[i], ptoken);
            result++;
            i++;
        } else {
            break;
        }
    }

    return result;
}

StatusType do_order(CmdInfo cmdInfo, MODE *p_mode, int paramNum, char **paramsArray, sudokoBoard *p_board) {
    StatusType status;

    status = cmdInfo.fun_ptr(paramsArray, p_board, p_mode, paramNum);
    return status;
}

int str_compare(const char *first, char *second) {
    int i;
    for (i = 0; first[i] == second[i]; i++) {
        if (first[i] == '\0') {
            return 0;
        }
    }
    return 1;
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
                      *board_ptr);/*checks inner validation inside any function. returns TRUE if game is over else FALSE*/

    return status;
}



