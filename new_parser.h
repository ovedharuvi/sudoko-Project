//
// Created by sharony on 8/17/2019.
//

#ifndef SUDOKOPROJECT_NEW_PARSER_H
#define SUDOKOPROJECT_NEW_PARSER_H

#endif //SUDOKOPROJECT_NEW_PARSER_H
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "game.h"


typedef enum{line_limit = 1, invalid_cmd = 2, invalid_mode = 3, invalid_param_num = 4, invalid_param_type = 5 }ErrorType;


#define BUFFERSIZE 256
#define MAX_PARAM 4
#define TEXT_SIZE 25
#define ORDERS_NUM 17

void welcome();
StatusType order(sudokoBoard **board, MODE mode);/* returns TRUE if the game is solved  successfully, FALSE otherwise, EXIT to finish the program*/
