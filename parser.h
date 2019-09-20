

#ifndef SUDOKOPROJECT_PARSER_H
#define SUDOKOPROJECT_PARSER_H
#include "game.h"
/* ----------PARSER MODULE----------
 * This module is responsible for understanding the user input and execute it accordingly.*/


/* -----Documentation of public Functions---------------------------------
The flow in the function 'order' is to get the user input, interpret it and process it.
In case of an error - break the flow.
 * -----------------------------------------------------------------------*/
StatusType order(sudokoBoard **board_ptr, MODE *p_mode);
/* returns TRUE if the game is solved  successfully, FALSE otherwise, EXIT to finish the program*/

#endif
