
#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "stack.h"
#ifndef SUDOKOPROJECT_SOLVER_H
#define SUDOKOPROJECT_SOLVER_H
int generate(sudokoBoard *board , int numOfLegalValues , int numOfEmptyCells);
int validate(sudokoBoard *board);
int guess(sudokoBoard *board , int threshold);
int guessHint(sudokoBoard *board , int row , int column);
int numOfSolutions(sudokoBoard *board); /*returns the number of solutions exist for a given board */
int checkIfValid(sudokoBoard *board , int value ,int  row ,int column); /*return 0 if value is valid return 1 otherwise*/

#endif //SUDOKOPROJECT_SOLVER_H
