#ifndef SUDOKOPROJECT_DEF_H
#define SUDOKOPROJECT_DEF_H


#include "board.h"

#define BUFFERSIZE 256
#define MAX_PARAM 4
#define TEXT_SIZE 25
#define ORDERS_NUM 17



/* The CmdInfo and CmdType helps to have generic validations of the commands.
 * The Enums are coordinated in their index:
 * The cmdArray initialize to have each cmdInfo of each command in the same range of the cmdType Enum.*/

typedef enum{
    INIT_MODE = 1,EDIT_MODE = 2,SOLVE_MODE = 3
}MODE;

typedef enum{
    String = 1, Integer = 2, Float = 3, None = 4
}ParamType;
typedef struct{
    StatusType (*fun_ptr)(char **paramsArray, sudokoBoard **board, MODE *p_mode, int paramNum);
    MODE mode;
    int paramNum;
    ParamType paramType;
    char * cmdName;
}CmdInfo;

extern  CmdInfo  CmdArray[ORDERS_NUM]; /*={Solve,Edit,Mark,Print,Set,Validate,Guess,Generate,Undo,Redo,Save,Hint,Guess_h,Num_s,Autofill,Reset_Game,Exit};*/

#endif
