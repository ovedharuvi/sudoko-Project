
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "LP.h"
#include "errors.h"
#ifndef SUDOKOPROJECT_SOLVER_H
#define SUDOKOPROJECT_SOLVER_H
sudokoBoard * generate(sudokoBoard *board , int numOfLegalValues , int numOfEmptyCells);
int validate(sudokoBoard *board);
sudokoBoard * guess(sudokoBoard *board , float threshold);
int guessHint(sudokoBoard *board , int row , int column);
int numOfSolutions(sudokoBoard *board); /*returns the number of solutions exist for a given board */
int isEmptySmallerThenLegalVal(sudokoBoard *board, int numOfLegalValues);
#endif //SUDOKOPROJECT_SOLVER_H
