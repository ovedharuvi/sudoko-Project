
#include <stdlib.h>
#include <stdio.h>
#include "gurobi_c.h"
#include "errors.h"
#ifndef SUDOKOPROJECT_LP_H
#define SUDOKOPROJECT_LP_H
typedef enum {
    UNSOLVABLE = 0, SOLVABLE = 1
} GUROBI_RETURN_VALUE;
int gurobi(sudokoBoard *sudokoBoard, float threshhold, CmdType command, int guessHintRow, int guessHintColumn);

#endif //SUDOKOPROJECT_LP_H
