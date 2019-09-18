
#include <stdlib.h>
#include <stdio.h>
#include "gurobi_c.h"
#include "errors.h"
#ifndef SUDOKOPROJECT_LP_H
#define SUDOKOPROJECT_LP_H
/*--------- LP MODULE ----------
 * This module includes all the linear programming related algorithms, from preparation the data for "Gurobi Optimizer"
 * to using it , and Decoding the given optimized solution according to command*/


/* Gurobi Function --
 *                      Return Value : UNSOLVABLE - if the board isn't solvable , SOLVABLE - otherwise
 *                      Param : sudokoBoard* board - Game board
 *                              float threshold - for GUESS command only -
 *                                                 this threshold will use as a lower bound for the guess,
     *                                             i.e after using the LP algorithm each value gets a score, and only values
     *                                             with a score greater than the threshold will be considered.
     *                          CmdType command - the function will use either LP or ILP according to command,
     *                                            and decodes the solution.
     *                          int guessHintRow , guessHintColumn -- the function will find potential values and their scores
     *                                                                  for cell - board[guesshintRow][guessHintColumn]*/
int gurobi(sudokoBoard *sudokoBoard, float threshold, CmdType command, int guessHintRow, int guessHintColumn);

#endif
