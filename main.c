
#include "parser.h"

int main() {
    StatusType game_stat , prog_stat;
    sudokoBoard** p_board;
    MODE mode;

    p_board = (sudokoBoard **) malloc(sizeof(sudokoBoard *));

    prog_stat = TRUE;
    SetCmdArray();/* init the CMD Array*/
    while (prog_stat!=EXIT) {/*program is not over*/
        mode = INIT_MODE;
        init_game();
        game_stat = FALSE;
        while (game_stat == FALSE){
            game_stat = order(p_board, &mode);
            if (game_stat==EXIT){
                prog_stat = EXIT;
            }
        }
    }
    exit_game(*p_board, TRUE);/*function that releases everything*/
    free(p_board);
    return 0;
}


