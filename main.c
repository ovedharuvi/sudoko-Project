
#include <fvec.h>
#include "new_parser.h"




int main() {
    StatusType game_stat , prog_stat;
    sudokoBoard* p_board = NULL;
    MODE mode;
    prog_stat = TRUE;

    while (prog_stat!=EXIT) {/*program is not over*/
        mode = INIT_MODE;
        init_game();
        game_stat = FALSE;
        while (game_stat == FALSE){
            game_stat = order(&p_board, &mode);
            if (game_stat==EXIT){
                prog_stat = EXIT;
            }
        }
    }
    exit_game(p_board);/*function that releases everything*/
    return 0;
}



