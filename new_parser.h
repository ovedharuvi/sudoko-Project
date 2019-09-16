//
// Created by sharony on 8/17/2019.
//

#ifndef SUDOKOPROJECT_NEW_PARSER_H
#define SUDOKOPROJECT_NEW_PARSER_H
#include "game.h"




void init_game();  /*initialize the command array and sends welcome message*/
StatusType order(sudokoBoard **board_ptr, MODE *p_mode);
/* returns TRUE if the game is solved  successfully, FALSE otherwise, EXIT to finish the program*/

#endif //SUDOKOPROJECT_NEW_PARSER_H
