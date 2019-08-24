//
// Created by sharony on 8/17/2019.
//

#include "new_parser.h"

StatusType get_line(char **pString);
StatusType error_message(ErrorType errorType, CmdInfo cmdInfo);
StatusType check_param_num(int cmd_index, int num_params_given, CmdInfo *cmdArray);
StatusType check_valid_params(char *paramArray[], CmdInfo cmdInfo);
StatusType do_order(CmdInfo cmdInfo, MODE mode, int paramNum, char *paramsArray[], sudokoBoard *p_board);
StatusType get_cmd(char **text, CmdInfo *cmdArray);
StatusType get_mode(int index, MODE Mode);
int get_params(char **text, char **paramsArray);
int error_line_limit();/*prints error_message and returns FALSE*/
int str_compare(const char *first, char *second);/*returns 0 if the second string is a prefix of the first string, else 1*/



void welcome() {
    printf("Welcome to Sudoku Game ! To initialize new board please enter 'edit'."
           "To load a game please enter 'edit' and then the path of your board.");

}

StatusType check_valid_params(char *paramArray[], CmdInfo cmdInfo) {
    int i,n,t;
    float f;
    ParamType paramType;
    int param_num = cmdInfo.paramNum;
    for(i = 0 ; i < param_num ; i++) {

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
    if ((CmdType)cmd_index == EDIT && num_params_given == 0)
        return TRUE;

    if(cmdArray[cmd_index].paramNum == num_params_given)
        return TRUE;
    else return FALSE;
}


StatusType get_line(char **pString){/*returns FALSE the user entered more than 256 characters, TRUE if valid 256 characters, EXIT if exit*/
    int c = 0, i = 0;
    fflush(stdin);

    while ( ( c = getchar() ) != EOF){
        if(i == BUFFERSIZE){
            return error_line_limit();
        }else if(c == '\n'){
            return TRUE;
        }else{
            *pString[i]=(char)c;
            i++;
        }
    }
    if(c == EOF){
        return EXIT;
    }
}


StatusType get_cmd(char **text, CmdInfo *cmdArray) {
    int i;
    char *pcommand;
    char delimit[] = " \t\r\n\v\f";
    pcommand = strtok(*text, delimit);

    for(i = 0 ; i < ORDERS_NUM ; i++){
        if(str_compare(cmdArray[i].cmdName,pcommand) == TRUE){
            return  i;
        }
    }
    return (int) INVALID;
         }

StatusType get_mode(int index, MODE Mode) {
    /*SOLVE = 0,EDIT = 1,MARK = 2, PRINT = 3, SET = 4, VALIDATE = 5, GUESS = 6, GENERATE = 7, UNDO = 8,
     REDO = 9, SAVE = 10, HINT = 11, GUESS_H = 12, NUM_S = 13, AUTOFILL = 14, RESET = 15, EXIT_GAME = 16, INVALID =17}CmdType*/
    return (int) CmdArray[index].mode == (int) Mode ? TRUE : FALSE;
}

StatusType error_message(ErrorType errorType, CmdInfo cmdInfo) {
    char str[TEXT_SIZE];
    char * ptoken = str;
    switch(errorType){
        case line_limit : printf("Error.You have entered over 256 characters.");break;
        case invalid_cmd : printf("Error. Invalid Command.");break;
        case invalid_mode :
            switch (cmdInfo.mode){
                case SOLVE_MODE: ptoken = "solve mode";break;
                case EDIT_MODE: ptoken = "edit mode"; break;
                case INIT_MODE: break;
                default: ptoken = "edit and solve modes";
            }

            printf("Error.Invalid MODE for This Command. The Command %s is available on %s only.",cmdInfo.cmdName,ptoken);
            break;
        case invalid_param_num : printf("Error.Invalid Numbers of Parameters for This Command.The Command %s needs %d Parameters."
                ,cmdInfo.cmdName,cmdInfo.paramNum);break;
        case invalid_param_type :
            switch (cmdInfo.paramType){
                case Integer: ptoken = "Integer"; break;
                case String: ptoken = "String"; break;
                case Float: ptoken = "Float"; break;
                case None: break;
            }

            printf("Error.Invalid Type of Parameters for This Command.The Command %s needs Parameters in Type of %s."
                    ,cmdInfo.cmdName,ptoken);break;
    }

    return FALSE;
}

int get_params(char **text, char **paramsArray) {
    char delimit[] = " \t\r\n\v\f";
    int i;
    int result = 0;
    char * ptoken;/*the pointer to be returned by strtok*/

    for(i = 0 ; i < MAX_PARAM ; i++){
        if((ptoken = strtok(*text, delimit))!= NULL)
            strcpy(paramsArray[i] ,ptoken);
            result += 1;
        }
    return result;
    }

StatusType do_order(CmdInfo cmdInfo, MODE mode, int paramNum, char **paramsArray, sudokoBoard *p_board) {
    cmdInfo.fun_ptr(paramsArray,p_board)
    return FALSE;
}

StatusType order(sudokoBoard **board, MODE mode) {
    char text[BUFFERSIZE] = {0};
    StatusType status;
    int cmd_index,param_num;
    char paramsArray [MAX_PARAM][BUFFERSIZE] ;
    status = get_line((char **) &text);
    if (status!= TRUE){/*makes the flow exit the game or restart the order*/
        return status;
    }
    cmd_index = get_cmd((char **) &text, CmdArray);
    if(cmd_index == (int) INVALID){
        return error_message(invalid_cmd,Solve);
    }
    status = get_mode(cmd_index, mode);/*checks whether this command is valid if cmd is edit or solve */
    if (status == FALSE){
        return error_message(invalid_mode,CmdArray[cmd_index]);
    }
    param_num = get_params((char **) &text,(char **) paramsArray);/*returns num of parameters given (above 3 it's 4)
 * and put the parameters in the variables*/
    status = check_param_num(cmd_index, param_num, CmdArray);/*checks if the num of parameters given matches the command.
 * if matches - returns TRUE. else returns FALSE */
    if (status == FALSE){
        return error_message(invalid_param_num,CmdArray[cmd_index]);
    }
    if(param_num!= 0) {
        status = check_valid_params((char **)paramsArray, CmdArray[cmd_index]);/*checks if the type of parameters given matches the command.
 * if matches - returns TRUE. else returns FALSE and prints error_message message.*/
        if (status == FALSE) {
            return error_message(invalid_param_type, CmdArray[cmd_index]);
        }
    }
    status = do_order(CmdArray[cmd_index], mode, param_num, (char **) paramsArray, *board);/*checks inner validation inside any function. returns TRUE if gameis over else FALSE*/
    if (status == TRUE){
        print_board();
        insert
    }
    return status;


}



