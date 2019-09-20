

#ifndef SUDOKOPROJECT_PARSER_H
#define SUDOKOPROJECT_PARSER_H
#include "game.h"
/* ----------PARSER MODULE----------
 * This module is responsible for understanding the user input and execute it accordingly.*/


/* -----Documentation of public Functions---------------------------------
The flow in the function 'order' is to get the user input, interpret it and process it.
 Each function in this flow returns StatusType : FALSE if failed else TRUE.
In case of an error - the flow breaks.
order return value is StatusType: TRUE if the game is solved  successfully, FALSE otherwise, EXIT to finish the program.

 Using CmdInfo and CmdType Enums gives the parser the option to do generic checks. */

 /*-----------------------------------------------------------------------*/
StatusType order(sudokoBoard **board_ptr, MODE *p_mode);


#endif
