//
// Created by sharony on 9/9/2019.
//

#ifndef SUDOKOPROJECT_DEF_H
#define SUDOKOPROJECT_DEF_H


#include "board.h"

#define BUFFERSIZE 256
#define MAX_PARAM 4
#define TEXT_SIZE 25
#define ORDERS_NUM 17
#define MAX_BOARD_SIZE 25

typedef enum {
    SOLVE = 0,EDIT = 1,MARK = 2, PRINT = 3, SET = 4, VALIDATE = 5, GUESS = 6, GENERATE = 7, UNDO = 8,
    REDO = 9, SAVE = 10, HINT = 11, GUESS_H = 12, NUM_S = 13, AUTOFILL = 14, RESET = 15, EXIT_GAME = 16, INVALID =17, MAX_CMD=INVALID
}CmdType;


typedef enum{
    INIT_MODE = 1,EDIT_MODE = 2,SOLVE_MODE = 3
}MODE;
typedef enum{
    EXIT = -1, FALSE = 0, TRUE = 1
}StatusType;
typedef enum{
    String = 1, Integer = 2, Float = 3, None = 4
}ParamType;
typedef struct{
    StatusType (*fun_ptr)(char **paramsArray, sudokoBoard *board, MODE *p_mode, int paramNum);
    MODE mode;
    int paramNum;
    ParamType paramType;
    char * cmdName;
}CmdInfo;


#endif //SUDOKOPROJECT_DEF_H
