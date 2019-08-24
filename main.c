#include <stdio.h>
#include "new_parser.h"




int main() {
    int game_stat = TRUE, prog_stat= TRUE;
    sudokoBoard** p_board = NULL;
    MODE *mode = NULL;
    *mode = INIT_MODE;
    while (prog_stat!=EXIT) {/*program is not over*/
        welcome();
        while (game_stat!= FALSE){
            game_stat = order(p_board,*mode);
            if (game_stat==EXIT){
                prog_stat = EXIT;
            }
        }
    }
    exit_program();/*function that releases everything*/
    return 0;
}



