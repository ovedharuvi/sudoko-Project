//
// Created by oved on 02/08/2019.
//

#include "game.h"

CmdInfo Solve ={&solve_cmd, INIT_MODE+EDIT_MODE+SOLVE_MODE , 1, String,"solve"};
CmdInfo Edit ={&edit_cmd, INIT_MODE+EDIT_MODE+SOLVE_MODE,1,String+None,"edit"};
CmdInfo Mark ={&mark_errors_cmd, SOLVE_MODE,1,Integer,"mark_errors"};
CmdInfo Print ={&print_board_cmd, EDIT_MODE+SOLVE_MODE,0,None,"print_board"};
CmdInfo Set ={&set_cmd, EDIT_MODE+SOLVE_MODE,3,Integer,"set"};
CmdInfo Validate ={&validate_cmd, EDIT_MODE+SOLVE_MODE,0, None, "validate"};
CmdInfo Guess ={&guess_cmd, SOLVE_MODE, 1, Float, "guess"};
CmdInfo Generate ={&generate_cmd, EDIT_MODE,2,Integer,"generate"};
CmdInfo Undo ={&undo_cmd, EDIT_MODE+SOLVE_MODE,0,None,"undo"};
CmdInfo Redo ={&redo_cmd, EDIT_MODE+SOLVE_MODE,0,None,"redo"};
CmdInfo Save ={&save_cmd, EDIT_MODE+SOLVE_MODE,1,String,"save"};
CmdInfo Hint ={&hint_cmd, SOLVE_MODE ,2,Integer,"hint"};
CmdInfo Guess_h ={&guess_h_cmd, SOLVE_MODE,2,Integer,"guess_hint"};
CmdInfo Num_s ={&num_s_cmd, EDIT_MODE+SOLVE_MODE,0,None,"num_solutions"};
CmdInfo Autofill ={&autofill_cmd, SOLVE_MODE,0,None,"autofill"};
CmdInfo Reset ={&reset_cmd, EDIT_MODE+SOLVE_MODE,0,None,"reset"};
CmdInfo Exit ={&exit_program_cmd, INIT_MODE+EDIT_MODE+SOLVE_MODE,0,None,"exit"};
CmdInfo  CmdArray[] ={Solve,Edit,Mark,Print,Set,Validate,Guess,Generate,Undo,Redo,Save,Hint,Guess_h,Num_s,Autofill,Reset,Exit};

int load(char *path) {
    return 0;
}

int is_game_over(sudokoBoard *board) {
    return NO;
}

int set(int i, int i1, int value, sudokoBoard *board) {
    return 0;
}


}
