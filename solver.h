
#endif //SUDOKOPROJECT_SOLVER_H
#include "stack.h"
#include "errors.h"
#include "LP.h"
#ifndef SUDOKOPROJECT_SOLVER_H
#define SUDOKOPROJECT_SOLVER_H
int generate(sudokoBoard *board , int numOfLegalValues , int numOfEmptyCells);
int validate(sudokoBoard *board);
int guess(sudokoBoard *board , float threshold);
int guessHint(sudokoBoard *board , int row , int column);
int numOfSolutions(sudokoBoard *board); /*returns the number of solutions exist for a given board */

#endif //SUDOKOPROJECT_SOLVER_H
