//
// Created by sharony on 9/9/2019.
//
#include "errors.h"
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
                default: ptoken = "edit and solve modes"; break;
            }
            printf("Error.Invalid MODE for This Command. The Command %s is available on %s only.",cmdInfo.cmdName,ptoken);
            break;
        case invalid_param_num :
            printf("Error.Invalid Numbers of Parameters for This Command.The Command %s needs %d Parameters."
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

            printf("Error.Invalid Type of Parameters for This Command.The Command %s needs Parameters in Type of %s."
                    ,cmdInfo.cmdName,ptoken);
            break;
        case board_errorneus:
            printf("Error.Invalid board for This Command.The Command %s needs board without errors.",cmdInfo.cmdName ) ;
            break;
        /*continue*/
        case incorrect_range:break;
        case illegal_value_for_board:break;
        case cmd_faild:break;
        case fixed_cell:break;
        case non_empty_cell:break;
        case unsolvable_board:break;
        case invalid_move:break;
        case no_moves_error:break;
        default:break;
    }

    return FALSE;
}
