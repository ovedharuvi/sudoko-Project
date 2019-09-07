#include "board.h"
#include <stdlib.h>
#include <stdio.h>
#include "gurobi_c.h"

#ifndef SUDOKOPROJECT_LP_H
#define SUDOKOPROJECT_LP_H
#define ILP 1
#define LP 0
typedef enum {
    SOLVE = 0, EDIT = 1, MARK = 2, PRINT = 3, SET = 4, VALIDATE = 5, GUESS = 6, GENERATE = 7, UNDO = 8,
    REDO = 9, SAVE = 10, HINT = 11, GUESS_H = 12, NUM_S = 13, AUTOFILL = 14, RESET = 15, EXIT_GAME = 16, INVALID = 17
} CmdType;
typedef enum {
    UNSOLVABLE = 0, SOLVABLE = 1, MEMORY_ERROR = 2, GUROBI_ERROR = 4
} GUROBI_RETURN_VALUE;

int getSolValues();
int isSolvable();
int gurobi(sudokoBoard *sudokoBoard, float threshhold, CmdType command, int guessHintRow, int guessHintColumn);

#endif //SUDOKOPROJECT_LP_H
