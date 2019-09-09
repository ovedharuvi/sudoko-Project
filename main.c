
#include "new_parser.h"




int main() {
    StatusType game_stat , prog_stat;
    sudokoBoard** p_board = NULL;
    MODE *mode = NULL;
    *mode = INIT_MODE;
    prog_stat= TRUE;
    while (prog_stat!=EXIT) {/*program is not over*/
        welcome();
        game_stat = FALSE;
        while (game_stat == FALSE){
            game_stat = order(p_board,mode);
            if (game_stat==EXIT){
                prog_stat = EXIT;
            }
        }

    }
    exit_program();/*function that releases everything*/
    return 0;
}



