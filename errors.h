//
// Created by sharony on 9/9/2019.
//
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "def.h"

#ifndef SUDOKOPROJECT_ERRORS_H
#define SUDOKOPROJECT_ERRORS_H

#endif //SUDOKOPROJECT_ERRORS_H

typedef enum{line_limit = 1, invalid_cmd = 2, invalid_mode = 3, invalid_param_num = 4
             , invalid_param_type = 5,incorrect_range=6,illegal_value_for_board=7,board_errorneus=8,cmd_faild=9
            , fixed_cell =10,non_empty_cell = 11, unsolvable_board = 12,invalid_move = 13,no_moves_error =14}ErrorType;

StatusType error_message(ErrorType errorType, CmdInfo cmdInfo);
