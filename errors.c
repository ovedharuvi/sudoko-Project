
#include <stdlib.h>
#include "errors.h"
StatusType error_message(ErrorType errorType, CmdInfo cmdInfo) {
    char str[TEXT_SIZE];
    char * ptoken = str;

    switch(errorType){
        case line_limit : printf("Error. You have entered over 256 characters.\n please enter a valid command :\n");break;
        case invalid_cmd : printf("Error. Invalid Command.\n");break;
        case invalid_mode :
            switch (cmdInfo.mode){
                case SOLVE_MODE: ptoken = "solve mode";break;
                case EDIT_MODE: ptoken = "edit mode"; break;
                case INIT_MODE: break;
                default: ptoken = "edit and solve modes"; break;
            }
            printf("Error. Invalid MODE for This Command. The Command %s is available on %s only.\n",cmdInfo.cmdName,ptoken);
            break;
        case invalid_param_num :
            printf("Error. Invalid Numbers of Parameters for This Command.\nThe Command %s needs %d Parameters.\n"
                    ,cmdInfo.cmdName,cmdInfo.paramNum);
            break;
        case invalid_param_type :
            switch (cmdInfo.paramType){
                case Integer: ptoken = "Integer"; break;
                case String: ptoken = "String"; break;
                case Float: ptoken = "Float"; break;
                case None: break;
                default:break;
            }

            printf("Error. Invalid Type of Parameters for This Command.\nThe Command %s needs Parameters in Type of %s\n."
                    ,cmdInfo.cmdName,ptoken);
            break;
        case board_erroneous:
            printf("Error. Invalid board for This Command.\nThe Command %s needs board without errors in this MODE.\n",cmdInfo.cmdName ) ;
            break;

        case incorrect_range:
            printf("Error. Incorrect range of the parameters.\n");
            break;
        case fixed_cell:
            printf("Error. this cell is fixed.\n");
            break;
        case non_empty_cell:
            printf("Error. this cell is not empty.\n");
            break;
        case unsolvable_board:
            printf("Error. Unsolvable board.\n");
            break;
        case no_moves_error:
            printf("Error. No more moves left to %s.\n",cmdInfo.cmdName);
            break;
        case gurobi_error:
            printf("Gurobi Error.\n");
            break;
        case memory_error:
            printf("The command %s failed because of memory error.\n", cmdInfo.cmdName);
            break;
        case file_error:
            printf("The command %s failed because of file loading error.\n", cmdInfo.cmdName);
            break;
        default:
            break;
    }

    return FALSE;
}



void myFree(void *pointer) {
    if (pointer!= NULL){
        free(pointer);
    }

}
