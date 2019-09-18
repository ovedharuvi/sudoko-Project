
#include <stdio.h>
#include "def.h"


#ifndef SUDOKOPROJECT_ERRORS_H
#define SUDOKOPROJECT_ERRORS_H
typedef enum{line_limit = 1, invalid_cmd = 2, invalid_mode = 3, invalid_param_num = 4
    , invalid_param_type = 5,incorrect_range=6,board_erroneous=8
    , fixed_cell =10,non_empty_cell = 11, unsolvable_board = 12,no_moves_error =14,gurobi_error = 15,memory_error = 16,file_error = 17}ErrorType;

StatusType error_message(ErrorType errorType, CmdInfo cmdInfo); /*sends error message due to the command*/

#endif

